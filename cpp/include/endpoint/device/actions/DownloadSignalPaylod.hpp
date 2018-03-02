// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef DOWNLOADSIGNALPAYLOD_HPP
#define DOWNLOADSIGNALPAYLOD_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class DownloadSignalPaylod : public ISkyDeviceAction
{
public:
    DownloadSignalPaylod(Listener* const _listener, const SignalData::Command _type);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    const SignalData::Command type;
    const SignalData::Command downloadCommand;

    enum State
    {
        IDLE,
        INITIAL_COMMAND,
        DATA_RECEPTION
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;

    SignalData::Command getDownloadCommand(void) const;
    DeviceEvent::Type getMonitorFailEvent(void) const;
};

#endif // DOWNLOADSIGNALPAYLOD_HPP
