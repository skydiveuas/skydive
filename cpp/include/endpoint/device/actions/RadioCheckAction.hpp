// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef RADIOCHECKACTION_HPP
#define RADIOCHECKACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class RadioCheckAction : public ISkyDeviceAction
{
public:
    RadioCheckAction(Listener* const _listener);

    void start(void) override;

    IMessage::MessageType getExpectedControlMessageType(void) const override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        INITIAL_COMMAND,
        CONTROLS_RECEPTION,
        RUNNING,
        BREAKING
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
    void handleUserEvent(const OperatorEvent& event) override;
};

#endif // RADIOCHECKACTION_HPP
