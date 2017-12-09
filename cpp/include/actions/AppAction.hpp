// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef APPACTION_HPP
#define APPACTION_HPP

#include "ICommAction.hpp"

#include <atomic>

class UserUavEventAction;
class UserUavEventUpload;
class UserUavEventDownload;

class AppAction : public ICommAction
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
    void handleUserEvent(const UserUavEvent& event) override;

    void handleUserUavEventAction(const UserUavEventAction& event);
    void handleUserUavEventUpload(const UserUavEventUpload& event);
    void handleUserUavEventDownload(const UserUavEventDownload& event);
};

#endif // APPACTION_HPP
