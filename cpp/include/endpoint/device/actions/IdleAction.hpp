// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef IDLEACTION_HPP
#define IDLEACTION_HPP

#include "ISkyDeviceAction.hpp"

class PilotEventConnect;

class IdleAction : public ISkyDeviceAction
{
public:
    IdleAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const;

private:
    void handleReception(const IMessage& message) override;
    void handleUserEvent(const PilotEvent& event) override;

    void handleDirectFlightCommand(const PilotEventConnect& ev);
};

#endif // IDLEACTION_HPP
