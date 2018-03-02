#include "ISkyDeviceMonitor.hpp"

ISkyDeviceMonitor::~ISkyDeviceMonitor()
{
}

void ISkyDeviceMonitor::notifyDeviceEvent(const DeviceEvent* const event)
{
    notifyDeviceEvent(std::unique_ptr<const DeviceEvent>(event));
}

double ISkyDeviceMonitor::getControlDataSendingFreq(void)
{
    // by default return 25 Hz's
    return 25.0;
}
