// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef SENSORSLOGGERACTION_HPP
#define SENSORSLOGGERACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class SensorsLoggerAction : public ISkyDeviceAction
{
public:
    SensorsLoggerAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    IMessage::MessageType getExpectedControlMessageType(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        INITIAL_COMMAND,
        LOGGING,
        BREAKING
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
    void handleUserEvent(const OperatorEvent& event) override;
};

#endif // SENSORSLOGGERACTION_HPP
