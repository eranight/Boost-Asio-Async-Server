#include "Client.h"

#include <iostream>
#include <boost/bind.hpp>

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
	cout << "start read" << endl;
	asio::async_read_until(socket, request, "\r\n",
		bind(&Client::onReadCallback, shared_from_this(), asio::placeholders::error, asio::placeholders::bytes_transferred)
	);
}

void Client::onReadCallback(const system::error_code & errorCode, size_t bytesNum)
{
	if (errorCode != 0)
		cerr << "read error: " << errorCode.message() << endl;
	else
	{
		std::istream streamRequest(&request);
		HttpEngine http(streamRequest);
		std::string fileName = http.getFileName();
		if (http.getRequestType() != HttpEngine::HttpType::GET || fileName.empty())
			response = http.getPageNotFoundResponse();
		else
		{
			responseFile.open(fileName, filesystem::ifstream::binary);
			if (responseFile.good())
				response = http.getGoodResponseHeader(fileName, filesystem::file_size(fileName));
			else
				response = http.getFileNotExistResponse(fileName);
		}
		asio::async_write(socket, asio::buffer(response),
			bind(&Client::onWriteCallback, shared_from_this(), asio::placeholders::error, asio::placeholders::bytes_transferred));
	}
}

void Client::onWriteCallback(const system::error_code & errorCode, size_t bytesNum)
{
	if (errorCode != 0)
	{
		cerr << "write error: " << errorCode.message() << endl;
		if (responseFile.is_open())
			responseFile.close();
	}
	else if (responseFile.is_open())
	{
		responseFile.read(responseBuffer.c_array(), responseBuffer.size());
		std::streamsize bytesCount = responseFile.gcount();
		if (bytesCount > 0)
			asio::async_write(socket, asio::buffer(responseBuffer.c_array(), bytesCount),
				bind(&Client::onWriteCallback, shared_from_this(), asio::placeholders::error, asio::placeholders::bytes_transferred));
		if (responseFile.eof())
		{
			responseFile.close();
			cout << "The file successfully sent!" << endl;
		}
	}
}
