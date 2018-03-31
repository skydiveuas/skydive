#include "SkyException.hpp"

#include <iostream>

std::vector<std::string> SkyException::trace;

// dummy constructor for another types of exceptions
SkyException::SkyException(void)
{
}

SkyException::SkyException(const char* _what, const char* _time, const char* _file, const int _line) :
    whatMessage(_what)
{
    trace.push_back(std::string(_time) + " - " +
        std::string(_what) + " @ " +
        std::string(_file) + " : " +
        std::to_string(static_cast<long long>(_line)));
}

SkyException::~SkyException()
{
}

#ifdef __GNUC__
const char* SkyException::what() const noexcept
{
    return whatMessage.c_str();
}
#elif _MSC_VER
const char* Exception::what() const //noexcept
{
    return whatMessage.c_str();
}
#endif

const std::string& SkyException::message(void) const
{
    return whatMessage;
}

const std::vector<std::string>& SkyException::getTrace(void)
{
    return trace;
}

void SkyException::printTrace(void)
{
    for (const std::string& tr : trace)
    {
        std::cout << tr << "\n";
    }
    trace.clear();
}
