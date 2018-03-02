// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef ISKYTIMER_HPP
#define ISKYTIMER_HPP

#include <functional>

class ISkyTimer
{
public:
    ISkyTimer(std::function<void(void)> _exec);

    virtual ~ISkyTimer(void);

    virtual void start(const double freqency) = 0;
    virtual void stop(void) = 0;

protected:
    void onTimeout(void) const;

private:
    std::function<void(void)> exec;
};

#endif // ISKYTIMER_HPP
