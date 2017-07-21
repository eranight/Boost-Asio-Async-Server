#include "HttpEngine.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <boost/lexical_cast.hpp>

using std::string;
using std::istream;
using std::ifstream;

const size_t HTTP_REQUEST_METHOD = 0;
const size_t HTTP_REQUEST_URL = 1;

const string pageNotFound =
"<!DOCTYPE html>" \
"<html>" \
"<head>" \
"<h1 align=\"center\">404 PAGE NOT FOUND</h1>" \
"</head>" \
"</html>";

const string pageNotFoundResponseString =
"HTTP/1.1 404 Not Found\r\n" \
"Content-Type: text/html\r\n" \
"Content-Length: " + boost::lexical_cast<string>(pageNotFound.size()) + "\r\n" \
"Connection: Closed\r\n" \
"\r\n" + pageNotFound;

const string fileNotExist =
"<!DOCTYPE html>" \
"<html>" \
"<head>" \
"<h1 align=\"center\">FILE DOES NOT EXIST</h1>" \
"</head>" \
"</html>";

const string fileNotExistResponseString =
"HTTP/1.1 200 OK\r\n" \
"Content-Type: text/html\r\n" \
"Connection: Closed\r\n" \
"Content-Length: ";

const string goodResponseHeader =
"HTTP/1.1 200 OK\r\n" \
"Connection: Closed\r\n" \
"Content-Type: application/octet-stream\r\n" \
"Content-Transfer-Encoding: Binary\r\n" \
"Content-Disposition: attachment; filename=\"";

HttpEngine::HttpEngine(istream & stream)
{
	std::copy(std::istream_iterator<string>(stream), std::istream_iterator<string>(), std::back_inserter(requestVector));
}

HttpEngine::HttpMethod HttpEngine::getRequestMethod()
{
	if (requestVector[HTTP_REQUEST_METHOD] == "GET")
		return HttpMethod::GET;
	return HttpMethod::OTHER;
}

string HttpEngine::getURL()
{
	return requestVector[HTTP_REQUEST_URL];
}

string HttpEngine::getPageNotFoundResponse()
{
	return pageNotFoundResponseString;
}

string HttpEngine::getFileNotExistResponse(const string & fileName)
{
	string strFileNotExist =
		"<!DOCTYPE html>" \
		"<html>" \
		"<head>" \
		"<h1 align=\"center\">THE FILE \"" + fileName + "\" DOES NOT EXIST</h1>" \
		"</head>" \
		"</html>";
	return fileNotExistResponseString +
		boost::lexical_cast<string>(strFileNotExist.size()) + "\r\n" \
		"\r\n" + strFileNotExist;
}

string HttpEngine::getGoodResponseHeader(const string & fileName, uintmax_t fileSize)
{
	string strGoodHeaderResponse = goodResponseHeader +
		fileName + "\"\r\n" \
		"Content-Length: " + boost::lexical_cast<string>(fileSize) + "\r\n" \
		"\r\n";
	return strGoodHeaderResponse;
}

std::ostream & operator<<(std::ostream & os, const HttpEngine & http)
{
	std::copy(http.requestVector.begin(), http.requestVector.end(), std::ostream_iterator<string>(os, "\r\n"));
	return os;
}
