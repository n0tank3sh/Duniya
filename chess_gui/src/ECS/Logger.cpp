#include "Logger.hpp"


void Logger::Log(std::string content)
{
	std::unique_lock<std::mutex> lock(mt);
	ss << content << std::endl;
}

void Logger::Paste()
{
	if(ss.rdbuf()->in_avail() != 0)
	{
		std::cout << ss.str();
		ss.clear();
	}
}
