// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef APPACTION_HPP
#define APPACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class PilotEventAction;
class PilotEventUpload;
class PilotEventDownload;

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
    void handleUserEvent(const PilotEvent& event) override;

    void handleUserUavEventAction(const PilotEventAction& event);
    void handleUserUavEventUpload(const PilotEventUpload& event);
    void handleUserUavEventDownload(const PilotEventDownload& event);
};

#endif // APPACTION_HPP
