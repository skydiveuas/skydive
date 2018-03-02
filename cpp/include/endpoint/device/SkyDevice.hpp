// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef SKYDIVEUAV_HPP
#define SKYDIVEUAV_HPP

#include "DeviceEvent.hpp"
#include "OperatorEvent.hpp"

#include "ISkyCommInterface.hpp"
#include "ISkyDeviceMonitor.hpp"

#include "ISkyDeviceAction.hpp"

#include "CommDispatcher.hpp"

#include <ctime>
#include <memory>
#include <atomic>

class SkyDevice :
        public ISkyCommInterface::Listener,
        public ISkyDeviceAction::Listener
{
public:
    SkyDevice(ISkyDeviceMonitor* const _monitor,
                  const double _pingFreq,
                  const double _controlFreq,
                  const double _connectionTimeout);

    /**
     * Sends event to ongoing UAV action. This is the main communication pipeline,
     * every command allowed and needed will be handled by calling this method.
     * If unexpected event will be pushed, communication will be broken
     * and UavEvent::ERROR_MESSAGE will be emmited by ISkyDiveUav interface.
     * Event should be dynamicaly allocated and will be deleted after processing.
     * Calling this method is thread safe.
     */
    void pushOperatorEvent(const OperatorEvent* const operatorEvent);

    /**
     * Memory safe implementation for raw pointer argument pushUserUavEvent method.
     */
    void pushOperatorEvent(std::unique_ptr<const OperatorEvent> operatorEvent);

    /**
     * Returns state of communication with UAV. Also this is state of UAV processing loop.
     * Evaluation of allowed UserUavEvent should be done by checking ongoing state.
     * Calling this method is thread safe.
     */
    ISkyDeviceAction::Type getState(void) const;

private:
    // listener for events emitted from UAV
    ISkyDeviceMonitor* const monitor;

    // used communication buffer
    ISkyCommInterface* interface;

    // preformed UAV action
    std::shared_ptr<ISkyDeviceAction> action;

    // dispatcher for SkyDive Comm Protocol
    CommDispatcher dispatcher;

    // timeing settings
    const double pingFreq, controlFreq;
    const double connectionTimeoutFreq;

    // timers for freqent tasks
    std::unique_ptr<ISkyTimer> pingTimer;
    std::unique_ptr<ISkyTimer> connetionTimer;

    // ping feature variables
    int sentPingValue;
    clock_t sentPingTime;

    // connection timeout variables
    std::atomic<bool> receptionFeed;
    std::atomic<bool> connectionLost;

    // static buffer for building messages with ISignalPaylodData objects
    // allocated for memory menagment optimization
    unsigned char messageBuildingBuffer[IMessage::MAX_DATA_SIZE];

    // notify methods, can be called from another thread so action
    // has to be locked under shared pointer
    void notifyOperatorEvent(std::shared_ptr<ISkyDeviceAction> actionLock,
                            const OperatorEvent* const operatorEvent);
    void notifyReception(std::shared_ptr<ISkyDeviceAction> actionLock,
                         const IMessage* const message);

    void handleError(const std::string& message);

    void pingTimerHandler(void);
    void handlePong(const SignalData& signalData) const;

    void connectionTimerHandler(void);

    // IAppCommInterface::Listener overrides
    void onConnected(void) override;
    void onDisconnected(void) override;
    void onError(const std::string& message) override;
    void onDataReceived(const unsigned char* data, const unsigned dataSize) override;

    // ICommAction::Listener overrides
    ISkyDeviceMonitor* getMonitor(void) override;
    void startAction(ISkyDeviceAction* action, bool immediateStart = true) override;
    void onPongReception(const SignalData& pong) override;
    void send(const IMessage& message) override;
    void send(const ISignalPayloadMessage& message) override;
    void enablePingTask(bool enabled) override;
    void enableConnectionTimeoutTask(bool enabled) override;
    void connectInterface(ISkyCommInterface* _interface) override;
    void disconnectInterface(void) override;
};

#endif // SKYDIVEUAV_HPP
