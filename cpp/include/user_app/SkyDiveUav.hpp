// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef SKYDIVEUAV_HPP
#define SKYDIVEUAV_HPP

#include "UavEvent.hpp"
#include "UserUavEvent.hpp"

#include "IAppCommInterface.hpp"
#include "ISkyDiveMonitor.hpp"

#include "ICommAction.hpp"

#include "CommDispatcher.hpp"

#include <ctime>
#include <memory>
#include <atomic>

class SkyDiveUav :
        public IAppCommInterface::Listener,
        public ICommAction::Listener
{
public:
    SkyDiveUav(ISkyDiveMonitor* const _monitor,
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
    void pushUserUavEvent(const UserUavEvent* const userUavEvent);

    /**
     * Memory safe implementation for raw pointer argument pushUserUavEvent method.
     */
    void pushUserUavEvent(std::unique_ptr<const UserUavEvent> userUavEvent);

    /**
     * Returns state of communication with UAV. Also this is state of UAV processing loop.
     * Evaluation of allowed UserUavEvent should be done by checking ongoing state.
     * Calling this method is thread safe.
     */
    ICommAction::Type getState(void) const;

private:
    // listener for events emitted from UAV
    ISkyDiveMonitor* const monitor;

    // used communication buffer
    IAppCommInterface* interface;

    // preformed UAV action
    std::shared_ptr<ICommAction> action;

    // dispatcher for SkyDive Comm Protocol
    CommDispatcher dispatcher;

    // timeing settings
    const double pingFreq, controlFreq;
    const double connectionTimeoutFreq;

    // timers for freqent tasks
    std::unique_ptr<IAppTimer> pingTimer;
    std::unique_ptr<IAppTimer> connetionTimer;

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
    void notifyUserUavEvent(std::shared_ptr<ICommAction> actionLock,
                            const UserUavEvent* const userUavEvent);
    void notifyReception(std::shared_ptr<ICommAction> actionLock,
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
    ISkyDiveMonitor* getMonitor(void) override;
    void startAction(ICommAction* action, bool immediateStart = true) override;
    void onPongReception(const SignalData& pong) override;
    void send(const IMessage& message) override;
    void send(const ISignalPayloadMessage& message) override;
    void enablePingTask(bool enabled) override;
    void enableConnectionTimeoutTask(bool enabled) override;
    void connectInterface(IAppCommInterface* _interface) override;
    void disconnectInterface(void) override;
};

#endif // SKYDIVEUAV_HPP
