#pragma once

#include <string>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class Client : public boost::enable_shared_from_this<Client>
{
private:
	boost::asio::streambuf request;
	std::string response;
	boost::asio::ip::tcp::socket socket;

	Client(boost::asio::io_service & ioService);
public:
	typedef boost::shared_ptr<Client> tptr;

	static tptr create(boost::asio::io_service & ioService);

	void start();

	boost::asio::ip::tcp::socket & getSocket() { return socket; }

private:
	void onReadCallback(const boost::system::error_code & errorCode, size_t bytesNum);
	void onWriteCallback(const boost::system::error_code & errorCode, size_t bytesNum);
};

