// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __I_SIGNAL_PAYLOAD_MESSAGE__
#define __I_SIGNAL_PAYLOAD_MESSAGE__

#include "IMessage.hpp"

#include "SignalData.hpp"

/**
 * =============================================================================================
 * interface for data containers sent over signal data messages payload
 * =============================================================================================
 */
class ISignalPayloadMessage : public IMessage
{
public:
	// IMessage override
    PreambleType getPreambleType(void) const;

    /**
     * serialize
     */
    virtual void serialize(unsigned char* dst) const = 0;

    /**
     * getDataSize
     */
	virtual unsigned getDataSize(void) const = 0;

    /**
     * getSignalDataType
     */
	virtual SignalData::Command getSignalDataType(void) const = 0;

    /**
     * getSignalDataCommand
     */
	virtual SignalData::Command getSignalDataCommand(void) const = 0;

    /**
     * getUploadAction
     */
    virtual SignalData::Command getUploadAction(void) const = 0;

    /**
     * getCrc
     */
    virtual unsigned getCrc(void) const = 0;

    /**
     * isValid
     */
	virtual bool isValid(void) const = 0;

    /**
     * clone
     */
    virtual ISignalPayloadMessage* clone(void) const = 0;

    /**
     * isSignalPayloadMessage
     */
    virtual bool isSignalPayloadMessage(void) const;

    /**
     * operator== equals
     */
    virtual bool operator==(const ISignalPayloadMessage& right) const;

    /**
     * MessagesBuilder
     */
    class MessagesBuilder
    {
    public:
        MessagesBuilder(const ISignalPayloadMessage* const _data);

        ~MessagesBuilder(void);

        /**
         * getMessagesCount
         */
        unsigned getMessagesCount(void) const;

        /**
         * hasNext
         */
        bool hasNext(void) const;

        /**
         * getNext
         */
        void getNext(unsigned char* message);

    private:
        const ISignalPayloadMessage* const data;

        const unsigned messagesCount;
        unsigned counter;

        unsigned char* buffer;
    };
};

#endif // __I_SIGNAL_PAYLOAD_MESSAGE__
