// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef ACCELCALIBACTION_HPP
#define ACCELCALIBACTION_HPP

#include "ISkyDeviceAction.hpp"

#include <atomic>

class AccelCalibAction : public ISkyDeviceAction
{
public:
    AccelCalibAction(Listener* const _listener);

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
        CALIBRATION_RECEPTION
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
};

#endif // ACCELCALIBACTION_HPP
