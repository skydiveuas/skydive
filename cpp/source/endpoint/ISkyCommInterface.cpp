#include "ISkyCommInterface.hpp"

ISkyCommInterface::Listener::~Listener(void)
{
}

ISkyCommInterface::~ISkyCommInterface(void)
{
}

void ISkyCommInterface::setListener(Listener* _listener)
{
    listener = _listener;
}

void ISkyCommInterface::onConnected(void)
{
    listener->onConnected();
}

void ISkyCommInterface::onDisconnected(void)
{
    listener->onDisconnected();
}

void ISkyCommInterface::onError(const std::string& message)
{
    listener->onError(message);
}

void ISkyCommInterface::onDataReceived(const unsigned char* data, const unsigned dataSize)
{
    listener->onDataReceived(data, dataSize);
}
