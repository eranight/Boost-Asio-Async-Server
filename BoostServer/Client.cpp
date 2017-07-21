#include "Client.h"

#include <iostream>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

#include "HttpEngine.h"

using namespace boost;

using asio::ip::tcp;
using std::cerr;
using std::cout;
using std::endl;

Client::Client(asio::io_service & ioService) :
	socket(ioService)
{

}

Client::tptr Client::create(asio::io_service & ioService)
{
	return tptr(new Client(ioService));
}

void Client::start()
{
	asio::async_read_until(socket, request, "\r\n",
		bind(&Client::onReadCallback, shared_from_this(), asio::placeholders::error, asio::placeholders::bytes_transferred)
	);
}

void Client::onReadCallback(const system::error_code & errorCode, size_t bytesNum)
{
	if (errorCode != 0)
		cerr << "The reading from the socket ended with error " << errorCode << endl;
	else
	{
		cout << "The reading from the socket ended successfully!" << endl;
		std::istream streamRequest(&request);
		HttpEngine http(streamRequest);
		if (http.getRequestType() != HttpEngine::HttpType::GET)
			response = http.getPageNotFoundResponse();
		else
		{
			std::string fileName = http.getFileName();
			if (fileName.empty())
				response = http.getPageNotFoundResponse();
			else if (boost::filesystem::exists(boost::filesystem::path(fileName)))
			{

			}
			else
				response = http.getFileNotExistResponse(fileName);
		}
		asio::async_write(socket, asio::buffer(response),
			bind(&Client::onWriteCallback, shared_from_this(), asio::placeholders::error, asio::placeholders::bytes_transferred)
		);
	}
}

void Client::onWriteCallback(const system::error_code & errorCode, size_t bytesNum)
{
	if (errorCode != 0)
		cerr << "The writing to the socket ended with error " << errorCode << endl;
	else
	{
		cout << "The writing to the socket ended successfully!" << endl;
		start();
	}
}