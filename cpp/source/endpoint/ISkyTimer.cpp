#include "IAppTimer.hpp"

IAppTimer::IAppTimer(std::function<void (void)> _exec):
    exec(_exec)
{
}

IAppTimer::~IAppTimer(void)
{
}

void IAppTimer::onTimeout(void) const
{
    exec();
}
