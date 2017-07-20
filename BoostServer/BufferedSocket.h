/*#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>

class BufferedSocket
{
private:
	std::unique_ptr<boost::asio::ip::tcp::socket> socket;
	boost::asio::streambuf request;
	boost::system::error_code ignoredError;

public:

	BufferedSocket(boost::asio::ip::tcp::socket * socketPtr) : socket(socketPtr) {}
	~BufferedSocket() { socket->shutdown(boost::asio::socket_base::shutdown_both); socket->close(); }

	void read(const boost::system::error_code & err, size_t bytes)
	{
		if (err) return;
		if (bytes > 0)
		{
			std::iostream requestStream(&request);
			boost::asio::write(*socket, boost::asio::buffer(getResponse(requestStream)), ignoredError);
		}
	}

	boost::asio::streambuf & getRequestStream() { return request; }

private:

	std::string getResponse(std::iostream & stream)
	{
		std::string strBuf;
		stream >> strBuf;
		stream >> strBuf;
		if (strBuf == "/")
			return std::string(
				"HTTP/1.1 200 OK\r\n" \
				"Connection: Keep-Alive\r\n" \
				"\r\n" \
				"<!DOCTYPE html>" \
				"<html>" \
				"<head>OK</head><body><p>Туда ли ты зашел, петушок?</p></body>" \
				"</html>");
		else
			return std::string(
				"HTTP/1.1 200 OK\r\n" \
				"Connection: Keep-Alive\r\n" \
				"\r\n");
	}

};*/