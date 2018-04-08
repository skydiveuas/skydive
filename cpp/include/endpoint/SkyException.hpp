// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __SKY_EXCEPTION_GUARD__
#define __SKY_EXCEPTION_GUARD__

#include <exception>
#include <vector>
#include <iostream>
#include <string>

#define __SKY_EXCEPTION__( _WHAT ) (throw SkyException( _WHAT, __TIME__, __FILE__, __LINE__))

/**
 * =============================================================================================
 * Exception
 * =============================================================================================
 */
class SkyException : public std::exception
{
protected:
    std::string whatMessage;

    static std::vector<std::string> trace;

	// dummy constructor for another types of exceptions
    SkyException(void);

public:
    SkyException(const char* what, const char* time, const char* file, const int line);
    virtual ~SkyException();

#ifdef __GNUC__
    virtual const char* what() const noexcept override;
#elif _MSC_VER
    virtual const char* what() const;
#endif

    virtual const std::string& message(void) const;

    static const std::vector<std::string>& getTrace(void);
    static void printTrace(void);
};

#endif // __SKY_EXCEPTION_GUARD__
