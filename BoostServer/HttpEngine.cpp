#include "HttpEngine.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <boost/lexical_cast.hpp>

using std::string;
using std::ifstream;

std::ofstream logFile("C:/log.txt", std::ofstream::app);

const string pageNotFound =
	"<!DOCTYPE html>" \
	"<html>" \
		"<head>" \
			"<h1 align=\"center\">404 PAGE NOT FOUND</h1>" \
		"</head>" \
	"</html>";

const string errorResponseString =
	"HTTP/1.1 404 Not Found\r\n" \
	"Content-Type: text/html\r\n" \
	"Content-Length: " + boost::lexical_cast<string>(pageNotFound.size()) + "\r\n" \
	"Connection: Closed\r\n" \
	"\r\n" + pageNotFound;

const string fileDoesNotExist =
	"<!DOCTYPE html>" \
	"<html>" \
		"<head>" \
			"<h1 align=\"center\">FILE DOES NOT EXIST</h1>" \
		"</head>" \
	"</html>";

const string badResponseString =
	"HTTP/1.1 200 OK\r\n" \
	"Content-Type: text/html\r\n" \
	"Content-Length: " + boost::lexical_cast<string>(fileDoesNotExist.size()) + "\r\n" \
	"Connection: Closed\r\n" \
	"\r\n" + fileDoesNotExist;

string HttpEngine::getHttpResponse(std::iostream & stream)
{
	string filePath;
	if (!checkHttpGetFormat(stream, filePath))
		return string();
	if (filePath.empty())
		return errorResponseString;
	ifstream file(filePath, std::ifstream::binary);
	if (file.good())
	{
		string goodResponseString = constuctGoodResponseString(filePath, file);
		file.close();
		return goodResponseString;
	}
	else
		return badResponseString;
}

bool HttpEngine::checkHttpGetFormat(std::iostream & stream, string & filePath)
{
	std::vector<string> logvector;
	std::copy(std::istream_iterator<string>(stream), std::istream_iterator<string>(), std::back_inserter(logvector));
	if (logvector[0] == "GET")
	{
		filePath = getCorrectFilePath(logvector[1]);
		return true;
	}
	return false;
}

string HttpEngine::constuctGoodResponseString(const string & filePath, std::ifstream & fileStream)
{
	fileStream.seekg(0, fileStream.end);
	int len = fileStream.tellg();
	fileStream.seekg(0, fileStream.beg);
	string goodResponseString = string(
		"HTTP/1.1 200 OK\r\n" \
		"Connection: Closed\r\n" \
		"Content-Type: application/octet-stream\r\n" \
		"Content-Disposition: attachment; filename=\"" + filePath + "\"\r\n" \
		"Content-Length: " + boost::lexical_cast<string>(len) + "\r\n" \
		"\r\n"
	);
	std::vector<char> bytes;
	bytes.reserve(len);
	char byte;
	while (!fileStream.eof())
		goodResponseString.push_back(fileStream.get());
	return goodResponseString;
}

string HttpEngine::getCorrectFilePath(string filePath)
{
	string result;
	string getstr = "/get/";
	if (filePath.find(getstr) == 0)
		result = filePath.substr(getstr.size());
	return result;
}
