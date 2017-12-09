// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef IDLEACTION_HPP
#define IDLEACTION_HPP

#include "ICommAction.hpp"

class IdleAction : public ICommAction
{
public:
    IdleAction(Listener* const _listener);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const;

private:
    void handleReception(const IMessage& message) override;
    void handleUserEvent(const UserUavEvent& event) override;
};

#endif // IDLEACTION_HPP
