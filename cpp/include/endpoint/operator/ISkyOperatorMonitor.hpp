// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef ISKYDIVEMONITOR_HPP
#define ISKYDIVEMONITOR_HPP

#include "IAppTimer.hpp"

#include "IMessage.hpp"

#include "UavEvent.hpp"
#include "ControlData.hpp"

#include <string>
#include <functional>
#include <memory>

/**
 * ISkyDiveMonitor
 */
class ISkyDiveMonitor
{
public:  
    virtual ~ISkyDiveMonitor();

    /**
     * notifyUavEvent
     */
    void notifyUavEvent(const UavEvent* const event);

    /**
     * notifyUavEvent
     */
    virtual void notifyUavEvent(std::unique_ptr<const UavEvent> event) = 0;

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
    virtual IAppTimer* createTimer(std::function<void(void)> exec) = 0;

    /**
     * trace
     */
    virtual void trace(const std::string& trace) = 0;
};

#endif // ISKYDIVEMONITOR_HPP
