#include "ISignalPayloadMessage.hpp"

#include <string.h>

IMessage::PreambleType ISignalPayloadMessage::getPreambleType(void) const
{
	return IMessage::SIGNAL;
}

bool ISignalPayloadMessage::isSignalPayloadMessage(void) const
{
    return true;
}

bool ISignalPayloadMessage::operator==(const ISignalPayloadMessage& right) const
{
    // data size and CRC has to be equals
    // most of cases will fail here cause of CRC
    if (this->getDataSize() != right.getDataSize()
            || this->getCrc() != right.getCrc())
    {
        return false;
    }
    // finally check serialized data to check for equality
    const unsigned size = this->getDataSize();
    unsigned char* leftArray = new unsigned char[size];
    unsigned char* rightArray = new unsigned char[size];
    this->serialize(leftArray);
    right.serialize(rightArray);
    bool result = true;
    for (unsigned i = 0; i < size; i++)
    {
        if (leftArray[i] != rightArray[i])
        {
            result = false;
            break;
        }
    }
    delete[] leftArray;
    delete[] rightArray;
    return result;
}

ISignalPayloadMessage::MessagesBuilder::MessagesBuilder(const ISignalPayloadMessage* const _data):
    data(_data),
    messagesCount((unsigned)((double)data->getDataSize() / IMessage::SIGNAL_DATA_PAYLOAD_SIZE + 0.99999)),
    counter(0),
    buffer(new unsigned char[data->getDataSize()])
{
    data->serialize(buffer);
}

ISignalPayloadMessage::MessagesBuilder::~MessagesBuilder(void)
{
    delete[] buffer;
}

unsigned ISignalPayloadMessage::MessagesBuilder::getMessagesCount(void) const
{
	return messagesCount;
}

bool ISignalPayloadMessage::MessagesBuilder::hasNext(void) const
{
    return counter < messagesCount;
}

void ISignalPayloadMessage::MessagesBuilder::getNext(unsigned char* message)
{
    // preamble
    const unsigned char preambleChar = IMessage::getPreambleCharByType(IMessage::SIGNAL);
    for (unsigned i = 0; i < IMessage::PREAMBLE_SIZE - 1; i++)
    {
        message[i] = preambleChar;
    }
    message[IMessage::PREAMBLE_SIZE - 1] = 0;

    // command
    const int command = (int)data->getSignalDataType();
    memcpy(message + IMessage::PREAMBLE_SIZE, &command, 4);

    // max packets
    memcpy(message + IMessage::PREAMBLE_SIZE + 4, &messagesCount, 2);

    // actual packet
    memcpy(message + IMessage::PREAMBLE_SIZE + 4 + 2, &counter, 2);

    // payload
    if (counter == messagesCount - 1)
    {
        // last packet, copy only bytes that left in data buffer
        const unsigned sent = (messagesCount - 1) * IMessage::SIGNAL_DATA_PAYLOAD_SIZE;
        const unsigned left = data->getDataSize() - sent;
        memcpy(message + IMessage::PREAMBLE_SIZE + IMessage::SIGNAL_CONSTRAINT_SIZE,
            buffer + counter * IMessage::SIGNAL_DATA_PAYLOAD_SIZE,
            left);
    }
    else
    {
        memcpy(message + IMessage::PREAMBLE_SIZE + IMessage::SIGNAL_CONSTRAINT_SIZE,
            buffer + counter * IMessage::SIGNAL_DATA_PAYLOAD_SIZE,
            IMessage::SIGNAL_DATA_PAYLOAD_SIZE);
    }

    // CRC
    const unsigned short crcValue = IMessage::computeCrc16(message + IMessage::PREAMBLE_SIZE,
        SignalData::SIGNAL_CONSTRAINT_SIZE + SignalData::SIGNAL_DATA_PAYLOAD_SIZE);
    message[IMessage::SIGNAL_DATA_MESSAGE_SIZE - 2] = (unsigned char)(crcValue & 0xff);
    message[IMessage::SIGNAL_DATA_MESSAGE_SIZE - 1] = (unsigned char)((crcValue >> 8) & 0xff);

    counter++;
}
