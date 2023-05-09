#include "Base/base.hpp"

int main()
{
	boost::asio::io_service ios;
	server ser(ios, SERVER_PORT);
	ser.accept();
	ios.run();

	system("pause");
	return 0;
}
