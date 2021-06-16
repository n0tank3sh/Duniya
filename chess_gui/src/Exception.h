#pragma once
#include <exception>
#include <string>
#include <cstdint>

class CException : public std::exception
{
public:
    const char* what() const noexcept override;

    uint32_t GetLine() const noexcept;
    const char* GetFile() const noexcept;

    virtual std::string GetType() const noexcept;
    virtual std::string GetOriginalString() const noexcept;
private:
    uint32_t line;
    const char* file; 
};

