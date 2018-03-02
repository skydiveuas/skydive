// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef DISCONNECTACTION_HPP
#define DISCONNECTACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class DisconnectAction : public ISkyDeviceAction
{
public:
    DisconnectAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        DISCONNECTING
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
};

#endif // DISCONNECTACTION_HPP
