// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef FLIGHTACTION_HPP
#define FLIGHTACTION_HPP

#include "ISkyDeviceAction.hpp"

#include "communication/AutopilotData.hpp"

#include <atomic>

class PilotEventAutopilot;

class FlightAction : public ISkyDeviceAction
{
public:
    FlightAction(Listener* const _listener, const double _controlFreq);
    ~FlightAction(void);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        FLING,
        BREAKING,
    };

    const double controlFreq;

    std::atomic<State> state;

    ISkyTimer* controlTimer;

    void handleReception(const IMessage& message) override;
    void handleUserEvent(const PilotEvent& event) override;

    void handleRunningReception(const IMessage& message);
    void handleAutopilotReception(const AutopilotData& message);

    void controlTaskHandler(void);

    void flightEnded(const bool byBoard);

    void sendAutopilotTarget(const PilotEventAutopilot& event);
    void sendBaseConfirmation(const AutopilotData& base);
};

#endif // FLIGHTACTION_HPP
