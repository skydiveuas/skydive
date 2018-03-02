#include "ISkyDiveMonitor.hpp"

ISkyDiveMonitor::~ISkyDiveMonitor()
{
}

void ISkyDiveMonitor::notifyUavEvent(const UavEvent* const event)
{
    notifyUavEvent(std::unique_ptr<const UavEvent>(event));
}

double ISkyDiveMonitor::getControlDataSendingFreq(void)
{
    // by default return 25 Hz's
    return 25.0;
}
