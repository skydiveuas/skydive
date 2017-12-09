// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef ESCCALIB_HPP
#define ESCCALIB_HPP

#include "ICommAction.hpp"

#include <atomic>

class EscCalibAction : public ICommAction
{
public:
    EscCalibAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

protected:
    void proceed(const Parameter userCommand);

private:
    enum State
    {
        IDLE,
        INITIAL_COMMAND,
        ESC_DISCONNECT,
        ESC_DISCONNECT_ACK,
        ESC_CONNECT,
        ESC_CONNECT_ACK,
        CALIBRATING,
        FINAL_ESC_DISCONNECT,
        FINAL_ESC_DISCONNECT_ACK
    };

    std::atomic<State> state;

    std::atomic<bool> breaking;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;
    void handleUserEvent(const UserUavEvent& event) override;
};

#endif // ESCCALIB_HPP
