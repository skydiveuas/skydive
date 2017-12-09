// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef ICOMMACTION_HPP
#define ICOMMACTION_HPP

#include "IMessage.hpp"

#include "ISkyDiveMonitor.hpp"

#include "IAppCommInterface.hpp"

#include <atomic>
#include <memory>

class UserUavEvent;

class ICommAction
{
public:
    enum Type
    {
        IDLE_ACTION,
        CONNECT,
        UPGRADE,
        WHO_AM_I,
        DISCONNECT,
        APP,
        FLIGHT_INITIALIZATION,
        FLIGHT,
        ACCEL_CALIB,
        MAGNET_CALIB,
        UPLOAD_SIGNAL_PAYLOAD,
        DOWNLOAD_SIGNAL_PAYLOAD,
        SENSORS_LOGGER,
        RADIO_CHECK,
        RADIO_CALIB,
        ESC_CALIB,
        RESET
    };

    class Listener
    {
    public:
        virtual ~Listener(void);

        virtual ISkyDiveMonitor* getMonitor(void) = 0;

        virtual void startAction(ICommAction* action, bool immediateStart = true) = 0;

        virtual void onPongReception(const SignalData& pong) = 0;

        virtual void onError(const std::string& message) = 0;

        virtual void send(const IMessage& message) = 0;
        virtual void send(const ISignalPayloadMessage& message) = 0;

        virtual void enablePingTask(bool enabled) = 0;
        virtual void enableConnectionTimeoutTask(bool enabled) = 0;

        virtual void connectInterface(IAppCommInterface* interface) = 0;
        virtual void disconnectInterface(void) = 0;
    };

    ICommAction(Listener* const _listener);
    virtual ~ICommAction(void);

    virtual void start(void) = 0;
    virtual void end(void);

    void baseHandleReception(std::unique_ptr<const IMessage> message);

    virtual void handleReception(const IMessage& message) = 0;
    virtual void handleUserEvent(const UserUavEvent& event);

    virtual bool isActionDone(void) const = 0;

    virtual IMessage::MessageType getExpectedControlMessageType(void) const;

    virtual Type getType(void) const = 0;

    virtual std::string getStateName(void) const = 0;

    std::string getName(void) const;

    static std::string toString(const Type type);

protected:
    static const unsigned DEFAULT_SIGNAL_TIMEOUT = 1000; // [ms]
    static const unsigned MAX_SIGNAL_PAYLOAD_RECEPTION_ERRORS = 3; // [ms]

    typedef SignalData::Command Command;
    typedef SignalData::Parameter Parameter;

    // communication consumers
    Listener* const listener;
    ISkyDiveMonitor* const monitor;

    // signal timeouted communication variables
    std::atomic<bool> wasSignalReceptionProcedure;
    std::atomic<bool> wasSignalPayloadReceptionProcedure;

    SignalData sentSignal;
    Command expectedSignalCommand;

    Command receivedSignalPayload;
    unsigned receptionErrors;

    IAppTimer* signalTimer;

    void baseHandleTimeout(void);

    virtual void handleReception(const SignalData& message);
    virtual void handleSignalReception(const Parameter parameter);
    virtual void handleTimeout(void);

    void sendSignal(const Command command, const Parameter parameter,
                    const unsigned timeout = DEFAULT_SIGNAL_TIMEOUT);
    void startSignalTimeout(const Command expectedCommand,
                            const unsigned timeout = DEFAULT_SIGNAL_TIMEOUT);
    void startSignalTimeoutTimer(const unsigned timeout = DEFAULT_SIGNAL_TIMEOUT);

    void endSignalTimeout(void);

    void initializeSignalPayloadReception(const SignalData::Command& command);
    bool handleSignalPayloadReception(const IMessage& message);

    void handleIdleReception(const IMessage& message) const;

    bool isPingMessage(const IMessage& received, int& value) const;
    bool isPingMessage(const IMessage& received) const;

    bool matchSignalData(const SignalData& expected, const IMessage& received) const;
    bool matchSignalData(const SignalData::Command command,
                         const SignalData::Parameter parameter,
                         const IMessage& received) const;

    void except(const std::string& message) const;
    void except(const std::string& message, const IMessage& received) const;
    void except(const std::string& message, const UserUavEvent& event) const;
    void except(const std::string& message, const SignalData::Parameter parameter) const;
};

#endif // ICOMMACTION_HPP
