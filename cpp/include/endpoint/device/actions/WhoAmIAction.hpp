// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef WHOAMIACTION_HPP
#define WHOAMIACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class WhoAmIAction : public ISkyDeviceAction
{
public:
    WhoAmIAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        INITIAL_COMMAND
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
};

#endif // WHOAMIACTION_HPP
