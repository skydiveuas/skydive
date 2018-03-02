// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef CONNECTACTION_HPP
#define CONNECTACTION_HPP

#include "ICommAction.hpp"

#include "IAppCommInterface.hpp"

#include <atomic>

class ConnectAction : public ICommAction
{
public:
    ConnectAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        INITIAL_COMMAND,
        CALIBRATION,
        CALIBRATION_RECEPTION,
        FINAL_COMMAND,
    };

    std::atomic<State> state;

    bool wasNonStatic;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
};

#endif // CONNECTACTION_HPP
