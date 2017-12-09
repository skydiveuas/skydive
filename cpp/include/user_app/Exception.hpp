// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __EXCEPTION_GUARD__
#define __EXCEPTION_GUARD__

#include <exception>
#include <vector>
#include <iostream>
#include <string>

#define __RL_EXCEPTION__( _WHAT ) (throw Exception( _WHAT, __TIME__, __FILE__, __LINE__))

/**
 * =============================================================================================
 * Exception
 * =============================================================================================
 */
class Exception : public std::exception
{
protected:
    std::string whatMessage;

    static std::vector<std::string> trace;

	// dummy constructor for another types of exceptions
    Exception(void);

public:
    Exception(const char* what, const char* time, const char* file, const int line);
    virtual ~Exception();

#ifdef __GNUC__
    virtual const char* what() const noexcept override;
#elif _MSC_VER
    virtual const char* what() const;
#endif

    virtual const std::string& message(void) const;

    static const std::vector<std::string>& getTrace(void);
    static void printTrace(void);
};

#endif // __EXCEPTION_GUARD__
