#include<iostream>
#include <boost/asio.hpp>
#include <string>
#include <boost/unordered_map.hpp>
using namespace boost::asio::ip;
using namespace std;
#define SERVER_PORT 7000
#define DATA_MAX_LEN 1024;
//���ݰ�ͷ
struct NetPacketHeader
{
	char cFlag;   //����ͷ�ı�־������䡮s��
	long nLenth; //Ҫ���͵����ݴ�С ������Ҫ�����ļ������nlenthֵ���ܴܺ�������long
	char cDataType;//����/���͵��������� a-�ļ��б� b-�ļ� c-�����ı�
	char sFileFormat[4];//������ļ������ＴΪ�ļ���ʽ��������ı����ݣ�������'\0'���
	char cSendDc; // У���� �������������-"s"  �ͻ��˷������-"c"
};

class serviceThreadPool
{
	//����io_service���̳߳�
	//1.��ʼ��һ��������io_service����io_service::work����ס�����䲻�˳�
	//2.��io_service�����ڲ�ͬ���߳��ÿ��һ���ͻ��ˣ�����һ��io_service
	std::vector<std::shared_ptr<boost::asio::io_service>> m_ioServiceList;
	std::vector<std::shared_ptr<boost::asio::io_service::work>> m_workList;
	//std::vector<std::thread> m_threadList;
	int m_poolSize;
	int m_io_service_pos; //���η���io_service
public:
	//�ڶ�˵Ļ����ϣ��̳߳��е�������õ��ں���
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
		//ͨ�������������io_service
		boost::asio::io_service& io_service = *m_ioServiceList[m_io_service_pos++];
		if (m_io_service_pos == m_poolSize - 1)m_io_service_pos = 0;
		return io_service;
	}

};

class rwhandler
{

#define MSG_HEAD_SIZE 16  //��ͷ��С
	//���ڶ�д����ʱ��socketҪ���д�����ĵݹ麯�����а󶨣�
	//������ﵥ��ʵ��һ����д�࣬��������ÿһ�����ӽ����Ŀͻ��˵Ķ�д�¼�
	tcp::socket m_socket;
	unsigned char* m_getData;
	int m_clientId;
	std::function<void(int)> m_callbackError; //�쳣�ص�����
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
		//�����Ƚ�����ͷ�����첽������������¼����жϰ�ͷ�Ƿ���ȷ����ȷ�����������
		//�ڶ�������첽����¼��У�����this->read();
		boost::asio::async_read(m_socket, boost::asio::buffer(m_getData, MSG_HEAD_SIZE),
			[=](const boost::system::error_code& ec, size_t size)
			{
				if (ec) {
					printf("read error! \n");
					handleError(ec);
					return;
				}
				//������Խ�����ͨ���ص����������ź���۴��ݻ�Ӧ�ò�
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
	//Ӧ�ò㣬Ҳ����server������SetCallBackError����ִ���쳣�ص��ĺ���
	template<typename F>
	void SetCallBackError(F f) {
		m_callbackError = f;
	}
	void handleError(const boost::system::error_code& ec)
	{
		//���������Բ�ͬ��ec�Ĵ���������쳣����
		printf("error! error value is: %d  %s \n", ec.value(), ec.message());
		//�쳣��Ϊ2��ʾ�첽����ȴ�ʱԶ�˹ر��׽���
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
#define MAX_CLINET_NUM 7000//�����������
	//��������m_rwIdListȥ����ͻ��˵�������ID��ÿ�����¿ͻ������ӽ������ʹ�m_rwIdList��ͷ��ȡһ��ID,������ID��m_rwIdList��ɾ��
	//����пͻ����쳣���������Ͽ�������ݿͻ���IDȥ�Ƴ�������е�rwhandle��Ȼ�󽫸�ID�Ż�m_rwIdList��β��
public:
	explicit server(boost::asio::io_service& ios, short port, int serPoolSize = 4) :m_ios(ios)
		, m_acceptor(ios, tcp::endpoint(tcp::v4(), port)),
		m_rwIdList(MAX_CLINET_NUM), m_servicePool(serPoolSize)
	{
		int current = 0;
		//��ʼ��m_rwIdList������MAX_CLINET_NUM������ID
		std::generate_n(m_rwIdList.begin(), MAX_CLINET_NUM, [&current] {return ++current; });
		m_servicePool.startPool(); //�����̳߳�
	}
	~server()
	{
		printf("server distroy! \n");
	}
	void accept()
	{
		//���ﲻ������ָ�룬ʹ��rwhandler rw;�ͻᱨ������������ɾ�����󡱵Ĵ���
		//����ָ�����뿪������ʱӦ��Ҳ�ǻ�������Ϊʲô���ᱨ���أ�
		//����Ϊ��async_accept��lambda�н�rw�����βδ��룬��ʱ��rw�����ü����ͼ�1�ˣ���˲�������
		//������ָ���������������ָ���ü����ͻ��1
		std::shared_ptr<rwhandler> rw = std::make_shared<rwhandler>(m_servicePool.get_io_service());
		rw->setClientID(m_rwIdList.front());  //����client ID
		m_rwIdList.pop_front();
		//�����쳣�ص�����
		rw->SetCallBackError([this](int id) {
			printf("start callback!  id:%d \n", id);
			printf("client callback thread id is:%d \n", std::this_thread::get_id());
			reSetRwID(id);
			});
		printf(" id:%d \n", rw->getClientID());
		printf("start accept  thread id �� %d  \n", std::this_thread::get_id());
		//���ﲶ���б����벶��rw,��������ָ��ͻ��Զ�����
		m_acceptor.async_accept(rw->getSock(), [this, rw](const boost::system::error_code& error) {
			if (error)
			{
				printf("------------------------acceptor error--------------------------------------");
				printf("error! error value is: %d  %s \n", error.value(), error.message());
				return;
			}

			printf("a client coming in! \n");
			printf("client adress is: %s  id: %d \n", rw->getSock().remote_endpoint().address(), rw->getClientID());
			//m_rwList.push_back(rw); //��Ȼ��������ע�͵�push_back����������ָ������������
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
	//��������ڹ����������Ŀͻ��˵Ķ�д
	boost::unordered_map<int, std::shared_ptr<rwhandler>> m_rwMap;
	std::list<int> m_rwIdList; //�������ӽ����Ŀͻ��˵�ID
	boost::asio::io_service& m_ios; //���io_serviceֻ����acceptor
	tcp::acceptor m_acceptor;
	serviceThreadPool m_servicePool;
	std::mutex m_lock;
};
