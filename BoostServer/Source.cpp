#include <iostream>
#include <boost/lexical_cast.hpp>

#include "Server.h"

using std::cerr;
using std::cout;
using std::endl;

int main(int argn, char * argc[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	if (argn != 2)
	{
		cerr << "invalide arguments number!" << endl;
		return 1;
	}

	int port;
	try
	{
		port = boost::lexical_cast<int>(argc[1]);
	}
	catch (boost::bad_lexical_cast &ex)
	{
		cerr << "invalid port format!" << endl;
		return 1;
	}
	if (port < 1 || port > 0xFFFF)
	{
		cerr << "invalid port number!" << endl;
		return 1;
	}

	try
	{
		boost::asio::io_service ioService;
		cout << "start server" << endl;
		Server server(ioService, port);
		ioService.run();
	}
	catch (std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	cout << "stop server" << endl;
	return 0;
}