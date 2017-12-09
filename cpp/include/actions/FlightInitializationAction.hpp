// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef FLIGHTINITIALIZATIONACTION_HPP
#define FLIGHTINITIALIZATIONACTION_HPP

#include "ICommAction.hpp"

#include <atomic>

class FlightInitializationAction : public ICommAction
{
public:
    FlightInitializationAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    enum State
    {
        IDLE,
        INITIAL_COMMAND,
        CONTROLS_RECEPTION,
        ROUTE_COMMAND,
        ROUTE_RECEPTION
    };

    std::atomic<State> state;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;

    void flightReady(void);
};

#endif // FLIGHTINITIALIZATIONACTION_HPP
