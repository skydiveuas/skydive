// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef ISKYDIVEMONITOR_HPP
#define ISKYDIVEMONITOR_HPP

#include "ISkyTimer.hpp"

#include "IMessage.hpp"

#include "DeviceEvent.hpp"
#include "ControlData.hpp"

#include <string>
#include <functional>
#include <memory>

/**
 * ISkyDiveMonitor
 */
class ISkyDeviceMonitor
{
public:  
    virtual ~ISkyDeviceMonitor();

    /**
     * notifyUavEvent
     */
    void notifyDeviceEvent(const DeviceEvent* const event);

    /**
     * notifyUavEvent
     */
    virtual void notifyDeviceEvent(std::unique_ptr<const DeviceEvent> event) = 0;

    /**
     * getControlDataSendingFreq
     */
    virtual double getControlDataSendingFreq(void);

    /**
     * getControlDataToSend
     */
    virtual ControlData getControlDataToSend(void) = 0;

    /**
     * createTimer
     */
    virtual ISkyTimer* createTimer(std::function<void(void)> exec) = 0;

    /**
     * trace
     */
    virtual void trace(const std::string& trace) = 0;
};

#endif // ISKYDIVEMONITOR_HPP
