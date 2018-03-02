#include "ISkyTimer.hpp"

ISkyTimer::ISkyTimer(std::function<void (void)> _exec):
    exec(_exec)
{
}

ISkyTimer::~ISkyTimer(void)
{
}

void ISkyTimer::onTimeout(void) const
{
    exec();
}
