#include<iostream>
#include <boost/asio.hpp>
#include <string>
#include <boost/unordered_map.hpp>
using namespace boost::asio::ip;
using namespace std;
#define SERVER_PORT 7000
#define DATA_MAX_LEN 1024;
//数据包头
struct NetPacketHeader
{
	char cFlag;   //数据头的标志——填充‘s’
	long nLenth; //要发送的数据大小 由于需要发送文件，因此nlenth值可能很大，所以用long
	char cDataType;//接收/发送的数据类型 a-文件列表 b-文件 c-数据文本
	char sFileFormat[4];//如果是文件，这里即为文件格式，如果是文本数据，这里用'\0'填充
	char cSendDc; // 校验码 服务器发包填充-"s"  客户端发包填充-"c"
};

class serviceThreadPool
{
	//管理io_service的线程池
	//1.初始化一定数量的io_service，用io_service::work阻塞住，让其不退出
	//2.让io_service运行在不同的线程里，每来一个客户端，分配一个io_service
	std::vector<std::shared_ptr<boost::asio::io_service>> m_ioServiceList;
	std::vector<std::shared_ptr<boost::asio::io_service::work>> m_workList;
	//std::vector<std::thread> m_threadList;
	int m_poolSize;
	int m_io_service_pos; //依次分配io_service
public:
	//在多核的机器上，线程池中的数量最好等于核数
	explicit serviceThreadPool(int pool_size = 4) :m_io_service_pos(0)
	{
		m_poolSize = pool_size > 0 ? pool_size : 1;
		for (int i = 0; i < m_poolSize; i++)
		{
			std::shared_ptr<boost::asio::io_service> io_service(new boost::asio::io_service);
			std::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*io_service));
			m_ioServiceList.push_back(io_service);
			m_workList.push_back(work);
		}
	}
	void startPool()
	{
		for (int i = 0; i < m_poolSize; i++) {

			std::thread runThread([=] {
				m_ioServiceList[i]->run();
				});
			runThread.detach();
		}
	}
	void stop()
	{
		for (int i = 0; i < m_poolSize; i++)
		{
			m_ioServiceList[i]->stop();
		}
	}
	boost::asio::io_service& get_io_service()
	{
		//通过这个函数分配io_service
		boost::asio::io_service& io_service = *m_ioServiceList[m_io_service_pos++];
		if (m_io_service_pos == m_poolSize - 1)m_io_service_pos = 0;
		return io_service;
	}

};

class rwhandler
{

#define MSG_HEAD_SIZE 16  //包头大小
	//由于读写操作时，socket要与读写操作的递归函数进行绑定，
	//因此这里单独实现一个读写类，用来处理，每一个连接进来的客户端的读写事件
	tcp::socket m_socket;
	unsigned char* m_getData;
	int m_clientId;
	std::function<void(int)> m_callbackError; //异常回调函数
public:
	explicit rwhandler(boost::asio::io_service& ios) :m_socket(ios)
	{
		m_getData = new unsigned char[1024];
		printf("rwhandler create \n");
	}
	~rwhandler() {
		delete[]m_getData;
		printf("rwhandler distroy \n");
	}
	tcp::socket& getSock()
	{
		return m_socket;
	}
	void read()
	{
		printf("read \n");
		//可以先解析包头，在异步读操作的完成事件中判断包头是否正确，正确则继续读包体
		//在读包体的异步完成事件中，继续this->read();
		boost::asio::async_read(m_socket, boost::asio::buffer(m_getData, MSG_HEAD_SIZE),
			[=](const boost::system::error_code& ec, size_t size)
			{
				if (ec) {
					printf("read error! \n");
					handleError(ec);
					return;
				}
				//这里可以将数据通过回调函数或者信号与槽传递回应用层
				cout<< string((char*)m_getData,MSG_HEAD_SIZE)<<endl;
				NetPacketHeader* p = (NetPacketHeader*)m_getData;
				printf(" %c %d %c %s %c %d \n", p->cFlag, p->nLenth, p->cDataType, p->sFileFormat, p->cSendDc, size);
				printf("client read thread id is: %d \n", std::this_thread::get_id());
				this->read();
			});
	}
	void setClientID(int id) {
		m_clientId = id;
	}
	int getClientID() {
		return m_clientId;
	}
	//应用层，也就是server中利用SetCallBackError设置执行异常回调的函数
	template<typename F>
	void SetCallBackError(F f) {
		m_callbackError = f;
	}
	void handleError(const boost::system::error_code& ec)
	{
		//这里可以针对不同的ec的错误码进行异常解析
		printf("error! error value is: %d  %s \n", ec.value(), ec.message());
		//异常码为2表示异步任务等待时远端关闭套接字
		if (ec.value() == 2) {
			//m_socket.close();
			boost::system::error_code ec2;
			m_socket.shutdown(tcp::socket::shutdown_send, ec2);
			m_socket.close(ec2);
			printf("clinet close the connected \n");
			if (m_callbackError)
				m_callbackError(m_clientId);
		}
	}
};


class server
{
#define MAX_CLINET_NUM 7000//最大连接数量
	//这里利用m_rwIdList去管理客户端的无序表的ID，每次有新客户端连接进来，就从m_rwIdList的头部取一个ID,并将该ID从m_rwIdList中删除
	//如果有客户端异常或者主动断开，则根据客户端ID去移除无序表中的rwhandle，然后将该ID放回m_rwIdList的尾部
public:
	explicit server(boost::asio::io_service& ios, short port, int serPoolSize = 4) :m_ios(ios)
		, m_acceptor(ios, tcp::endpoint(tcp::v4(), port)),
		m_rwIdList(MAX_CLINET_NUM), m_servicePool(serPoolSize)
	{
		int current = 0;
		//初始化m_rwIdList，产生MAX_CLINET_NUM数量的ID
		std::generate_n(m_rwIdList.begin(), MAX_CLINET_NUM, [&current] {return ++current; });
		m_servicePool.startPool(); //开启线程池
	}
	~server()
	{
		printf("server distroy! \n");
	}
	void accept()
	{
		//这里不用智能指针，使用rwhandler rw;就会报“尝试引用已删除对象”的错误
		//智能指针在离开作用于时应该也是会析构，为什么不会报错呢？
		//是因为在async_accept的lambda中将rw当作形参传入，这时候rw的引用计数就加1了，因此不会析构
		//将智能指针放入容器后，智能指引用计数就会加1
		std::shared_ptr<rwhandler> rw = std::make_shared<rwhandler>(m_servicePool.get_io_service());
		rw->setClientID(m_rwIdList.front());  //设置client ID
		m_rwIdList.pop_front();
		//设置异常回调函数
		rw->SetCallBackError([this](int id) {
			printf("start callback!  id:%d \n", id);
			printf("client callback thread id is:%d \n", std::this_thread::get_id());
			reSetRwID(id);
			});
		printf(" id:%d \n", rw->getClientID());
		printf("start accept  thread id ： %d  \n", std::this_thread::get_id());
		//这里捕获列表，必须捕获rw,否则智能指针就会自动析构
		m_acceptor.async_accept(rw->getSock(), [this, rw](const boost::system::error_code& error) {
			if (error)
			{
				printf("------------------------acceptor error--------------------------------------");
				printf("error! error value is: %d  %s \n", error.value(), error.message());
				return;
			}

			printf("a client coming in! \n");
			printf("client adress is: %s  id: %d \n", rw->getSock().remote_endpoint().address(), rw->getClientID());
			//m_rwList.push_back(rw); //果然，这里在注释掉push_back操作后，智能指针随后就析构了
			{
				std::unique_lock<std::mutex> lock(this->m_lock);
				m_rwMap.insert(std::make_pair(rw->getClientID(), rw));
				printf("map size : %d \n", m_rwMap.size());
			}
			m_rwMap.insert(std::make_pair(rw->getClientID(), rw));
			printf("map size : %d \n", m_rwMap.size());
			printf("client have acceptor thread id is: %d \n", std::this_thread::get_id());
			rw->read();
			accept();
			});
	}

	void reSetRwID(int id)
	{
		std::unique_lock<std::mutex> lock(this->m_lock);
		auto it = m_rwMap.find(id);
		if (it != m_rwMap.end())
			m_rwMap.erase(it);
		m_rwIdList.push_back(id);
		printf("--------map size is: %d \n--------", m_rwMap.size());
	}
	size_t getClientNum() {
		return  m_rwMap.size();
	}

private:
	//std::vector<std::shared_ptr<rwhandler>> m_rwList;
	//无序表，用于管理连接来的客户端的读写
	boost::unordered_map<int, std::shared_ptr<rwhandler>> m_rwMap;
	std::list<int> m_rwIdList; //管理连接进来的客户端的ID
	boost::asio::io_service& m_ios; //这个io_service只用于acceptor
	tcp::acceptor m_acceptor;
	serviceThreadPool m_servicePool;
	std::mutex m_lock;
};
