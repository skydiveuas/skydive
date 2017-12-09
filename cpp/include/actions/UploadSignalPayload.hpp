// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef UPLOADSIGNALPAYLOAD_HPP
#define UPLOADSIGNALPAYLOAD_HPP

#include "ICommAction.hpp"

#include <atomic>

class UploadSignalPayload : public ICommAction
{
public:
    UploadSignalPayload(Listener* const _listener, const ISignalPayloadMessage& _data);

    void start(void) override;

    bool isActionDone(void) const override;

    Type getType(void) const override;

    std::string getStateName(void) const override;

private:
    const ISignalPayloadMessage& data;
    const SignalData::Command command;

    enum State
    {
        IDLE,
        INITIAL_COMMAND,
        UPLOAD
    };

    std::atomic<State> state;

    unsigned retransmissionCounter;

    void handleReception(const IMessage& message) override;
    void handleSignalReception(const Parameter parameter) override;

    SignalData::Command getUploadCommand(const IMessage::MessageType type) const;
};

#endif // UPLOADSIGNALPAYLOAD_HPP
