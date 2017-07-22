#pragma once

#include "Client.h"

class Server
{
private:
	boost::asio::ip::tcp::acceptor acceptor;

public:
	Server(boost::asio::io_service & ioService, int port);

private:
	void nextAccept();
	void handleAccept(Client::tptr client, const boost::system::error_code & errorCode);
};

