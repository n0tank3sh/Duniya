#include "Exception.h"
#include <sstream>

CException::CException(uint32_t line, const char* file)
    :
        line(line), file(file)
{
    std::ostringstream whatStream(whatBuffer);
    whatStream << GetType() << '\n'
        << "[LINE]: " << line << '\n'
        << "[FILE]: " << GetFile() << '\n'
        << GetOriginalString();
    whatBuffer= whatStream.str();
}

CException::CException(uint32_t line, const char* file, std::string type, std::string originalString)
{
    std::ostringstream whatStream(whatBuffer);
    whatStream << type << '\n'
        << "[LINE]: " << line << '\n'
        << "[FILE]: " << GetFile() << '\n'
        << originalString;
    whatBuffer= whatStream.str();

}

const char* CException::what() const noexcept 
{
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
    return "CException";
}

std::string CException::GetOriginalString()  noexcept
{
    return "";
}
