#include "Server.h"

#include <iostream>
#include <boost/bind.hpp>

using namespace boost;

using asio::ip::tcp;
using std::cerr;
using std::cout;
using std::endl;

Server::Server(asio::io_service & ioService, int port) :
	acceptor(ioService, tcp::endpoint(tcp::v4(), port))
{
	nextAccept();
}


Server::~Server()
{
}

void Server::nextAccept()
{
	shared_ptr<Client> client = Client::create(acceptor.get_io_service());
	acceptor.async_accept(client->getSocket(), bind(&Server::handleAccept, this, client, asio::placeholders::error));
}

void Server::handleAccept(Client::tptr client, const system::error_code & errorCode)
{
	cout << "the socket accepted" << endl;
	if (errorCode == 0)
		client->start();
	else
		cerr << errorCode.message() << endl;
	nextAccept();
}
