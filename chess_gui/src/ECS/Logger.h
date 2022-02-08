#pragma once
#include <iostream>
#include <mutex>
#include <sstream>

class Logger
{
	std::mutex mt;
	std::ostringstream ss;
	public:
		Logger() = default;
		~Logger() = default;
		void Log(std::string content);
		void Paste();
};
