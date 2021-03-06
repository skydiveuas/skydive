// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef SKYDIVEUAV_HPP
#define SKYDIVEUAV_HPP

#include "DeviceEvent.hpp"
#include "PilotEvent.hpp"

#include "endpoint/ISkyCommInterface.hpp"
#include "ISkyDeviceMonitor.hpp"

#include "actions/ISkyDeviceAction.hpp"

#include "communication/CommDispatcher.hpp"

#include <ctime>
#include <memory>
#include <mutex>
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
    void pushPilotEvent(const PilotEvent* const pilotEvent);

    /**
     * Memory safe implementation for raw pointer argument pushUserUavEvent method.
     */
    void pushPilotEvent(std::unique_ptr<const PilotEvent> pilotEvent);

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

    // preformed UAV action, device state
    // this is shared because when new action is starting
    // we need to prevent it from beeing destructed if
    // antoher thread is still working on this state
    std::shared_ptr<ISkyDeviceAction> action;
    std::mutex actionLock;

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
    void notifyPilotEvent(std::shared_ptr<ISkyDeviceAction> guard,
                             const PilotEvent* const operatorEvent);
    void notifyReception(std::shared_ptr<ISkyDeviceAction> guard,
                         const IMessage* const message);

    void handleError(const std::string& message);

    void pingTimerHandler(void);
    void handlePong(const SignalData& signalData) const;

    void connectionTimerHandler(void);

    // ISkyCommInterface::Listener overrides
    void onConnected(void) override;
    void onDisconnected(void) override;
    void onError(const std::string& message) override;
    void onReceived(const unsigned char* data, const size_t length) override;

    // ISkyDeviceAction::Listener overrides
    ISkyDeviceMonitor* getMonitor(void) override;
    bool setupProtocolVersion(const unsigned version) override;
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
