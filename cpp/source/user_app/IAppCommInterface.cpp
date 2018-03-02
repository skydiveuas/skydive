#include "IAppCommInterface.hpp"

IAppCommInterface::Listener::~Listener(void)
{
}

IAppCommInterface::~IAppCommInterface(void)
{
}

void IAppCommInterface::setListener(Listener* _listener)
{
    listener = _listener;
}

void IAppCommInterface::onConnected(void)
{
    listener->onConnected();
}

void IAppCommInterface::onDisconnected(void)
{
    listener->onDisconnected();
}

void IAppCommInterface::onError(const std::string& message)
{
    listener->onError(message);
}

void IAppCommInterface::onDataReceived(const unsigned char* data, const unsigned dataSize)
{
    listener->onDataReceived(data, dataSize);
}
