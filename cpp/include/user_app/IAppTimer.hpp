// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef IAPPTIMER_HPP
#define IAPPTIMER_HPP

#include <functional>

class IAppTimer
{
public:
    IAppTimer(std::function<void (void)> _exec);

    virtual ~IAppTimer(void);

    virtual void start(const double freqency) = 0;
    virtual void stop(void) = 0;

protected:
    void onTimeout(void) const;

private:
    std::function<void (void)> exec;
};

#endif // IAPPTIMER_HPP
