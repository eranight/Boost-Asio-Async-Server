#pragma once

#include <string>
#include <iostream>
#include <vector>

class HttpEngine
{
private:
	HttpEngine() {}
	std::vector<std::string> requestVector;

public:
	enum class HttpMethod { GET = 0, OTHER };
	HttpEngine(std::istream & stream);
	HttpMethod getRequestMethod();
	std::string getURL();

	std::string getPageNotFoundResponse();
	std::string getFileNotExistResponse(const std::string & fileName);
	std::string getGoodResponseHeader(const std::string & fileName, uintmax_t fileSize);
};