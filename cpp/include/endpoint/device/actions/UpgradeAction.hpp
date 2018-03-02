// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef UPGRADEACTION_HPP
#define UPGRADEACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class UpgradeAction : public ISkyDeviceAction
{
public:
    UpgradeAction(Listener* const _listener);

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

#endif // UPGRADEACTION_HPP
