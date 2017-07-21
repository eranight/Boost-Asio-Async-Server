#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class HttpEngine
{
private:
	HttpEngine() {}
	std::vector<std::string> requestVector;

public:
	enum class HttpType { GET = 0, OTHER };
	HttpEngine(std::istream & stream);
	HttpType getRequestType();
	std::string getFileName(std::string ignoredPrefix = "/get/");

	friend std::ostream & operator<<(std::ostream & os, const HttpEngine & http);

	std::string getPageNotFoundResponse();
	std::string getFileNotExistResponse(const std::string & fileName);
	std::string getGoodResponseHeader(const std::string & fileName, uintmax_t fileSize);
};