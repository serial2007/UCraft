#include <iostream>
#include <boost/asio.hpp>


int main(int argc, char* argv[])
{
    // 所有asio类都需要io_service对象
    boost::asio::io_service iosev;
    boost::asio::ip::tcp::acceptor acceptor(iosev, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 1000));
    while(1)
    {
        boost::asio::ip::tcp::socket socket(iosev);
        acceptor.accept(socket);
        std::cout << socket.remote_endpoint().address() << std::endl;
        // 向客户端发送hello world!
        boost::system::error_code ec;

            
        socket.write_some(boost::asio::buffer("hello world!"), ec);

        // 如果出错，打印出错信息
        if (ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            break;
        }
        // 与当前客户交互完成后循环继续等待下一客户连接
    }
    return 0;
}