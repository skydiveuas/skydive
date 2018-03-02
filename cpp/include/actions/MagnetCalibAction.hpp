// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef MAGNETCALIBACTION_HPP
#define MAGNETCALIBACTION_HPP

#include "ICommAction.hpp"

#include <atomic>

class MagnetCalibAction : public ICommAction
{
public:
    MagnetCalibAction(Listener* const _listener);

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
        CALIBRATION_RESULT,
        CALIBRATION_SKIP_RESULT,
        CALIBRATION_RECEPTION
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
    void handleUserEvent(const UserUavEvent& event) override;
};

#endif // MAGNETCALIBACTION_HPP
