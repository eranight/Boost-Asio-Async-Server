#pragma once

#include <string>
#include <iostream>
#include <fstream>

class HttpEngine
{
private:
	HttpEngine() {}

public:
	static std::string getHttpResponse(std::iostream & stream);

private:
	static bool checkHttpGetFormat(std::iostream & stream, std::string & filePath);
	static std::string constuctGoodResponseString(const std::string & filePath, std::ifstream & fileStream);
	static std::string getCorrectingFilePath(std::string filePath);
};

