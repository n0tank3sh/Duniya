#include "Exception.hpp"
#include <sstream>

CException::CException(uint32_t line, const char* file)
    :
        line(line), file(file), type("CException")
{}

CException::CException(uint32_t line, const char* file, std::string type, std::string originalString)
	:
		line(line), file(file), type(type), originalString(originalString) 
{}

const char* CException::what() const noexcept 
{
	GetOriginalString();
    return whatBuffer.c_str();
}

uint32_t CException::GetLine() const noexcept
{
    return line;
}

const char* CException::GetFile() const noexcept
{
    return file;
}

std::string CException::GetType() const noexcept
{
    return type;
}

std::string CException::GetOriginalString() const  noexcept
{	
	std::ostringstream oss;
	oss << GetType() << "\n"
		<< "[LINE] :" << GetLine() << "\n"
		<< "[FILE] :" << GetFile()  << "\n"
		<< originalString << std::endl;
	whatBuffer = oss.str();
    return whatBuffer;
}
