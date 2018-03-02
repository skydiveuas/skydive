// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef APPACTION_HPP
#define APPACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class UserUavEventAction;
class OperatorEventUpload;
class OperatorEventDownload;

class AppAction : public ISkyDeviceAction
{
public:
    AppAction(Listener* const _listener);

    void start(void) override;

    void end(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        APP_LOOP
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleUserEvent(const OperatorEvent& event) override;

    void handleUserUavEventAction(const UserUavEventAction& event);
    void handleUserUavEventUpload(const OperatorEventUpload& event);
    void handleUserUavEventDownload(const OperatorEventDownload& event);
};

#endif // APPACTION_HPP
