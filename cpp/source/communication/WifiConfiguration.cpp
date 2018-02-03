#include "WifiConfiguration.hpp"

#include <string.h>

WifiConfiguration::WifiConfiguration(void)
{
    routerName = NULL;
    routerPassword = NULL;
    routerIp = NULL;
    clientIp = NULL;
}

WifiConfiguration::WifiConfiguration(const WifiConfiguration& wifiConfiguration)
{
    routerName = NULL;
    routerPassword = NULL;
    routerIp = NULL;
    clientIp = NULL;
    *this = wifiConfiguration;
}

WifiConfiguration::WifiConfiguration(const unsigned char* const src)
{
    routerName = NULL;
    routerPassword = NULL;
    routerIp = NULL;
    clientIp = NULL;

    // parse sizes
    memcpy((unsigned char*)this + 4, src, SIZES_VALUES_SIZE);

    // parse strings
    setRouterName((const char*)src + SIZES_VALUES_SIZE,
                  routerNameSize);
    setRouterPassword((const char*)src + SIZES_VALUES_SIZE + routerNameSize,
                      routerPasswordSize);
    setRouterIp((const char*)src + SIZES_VALUES_SIZE + routerNameSize + routerPasswordSize,
                routerIpSize);
    setClientIp((const char*)src + SIZES_VALUES_SIZE + routerNameSize + routerPasswordSize + routerIpSize,
                clientIpSize);

    // parse CRC
    memcpy(&crcValue, src + getDataSize() - sizeof(unsigned), sizeof(unsigned));
}

WifiConfiguration::~WifiConfiguration(void)
{
    delete[] routerName;
    // clean memory under password array due to safty
    if (NULL != routerPassword)
    {
        for (unsigned int i = 0; i < routerPasswordSize; i++)
        {
            routerPassword[i] = '0';
        }
    }
    delete[] routerPassword;
    delete[] routerIp;
    delete[] clientIp;
}

void WifiConfiguration::serialize(unsigned char* dst) const
{
    // serialize sizes
    memcpy(dst, (unsigned char*)this + 4, SIZES_VALUES_SIZE);

    // serialize strings
    memcpy(dst + SIZES_VALUES_SIZE,
           routerName, routerNameSize);
    memcpy(dst + SIZES_VALUES_SIZE + routerNameSize,
           routerPassword, routerPasswordSize);
    memcpy(dst + SIZES_VALUES_SIZE + routerNameSize + routerPasswordSize,
           routerIp, routerIpSize);
    memcpy(dst + SIZES_VALUES_SIZE + routerNameSize + routerPasswordSize + routerIpSize,
           clientIp, clientIpSize);

    // serialize CRC
    memcpy(dst + getDataSize() - sizeof(unsigned), &crcValue, sizeof(unsigned));
}

unsigned WifiConfiguration::getDataSize(void) const
{
    // all strings size + sizes values + sizeof crcValue
    return routerNameSize
            + routerPasswordSize
            + routerIpSize
            + clientIpSize
            + SIZES_VALUES_SIZE
            + sizeof(unsigned);
}

SignalData::Command WifiConfiguration::getSignalDataType(void) const
{
    return SignalData::WIFI_CONFIGURATION_DATA;
}

SignalData::Command WifiConfiguration::getSignalDataCommand(void) const
{
    return SignalData::WIFI_CONFIGURATION;
}

SignalData::Command WifiConfiguration::getUploadAction(void) const
{
    return SignalData::CONFIGURE_WIFI;
}

IMessage::MessageType WifiConfiguration::getMessageType(void) const
{
    return IMessage::WIFI_CONFIGURATION;
}

unsigned WifiConfiguration::computeCrc() const
{
    unsigned char *dataTab = new unsigned char[getDataSize()];
    serialize(dataTab);
    unsigned result = IMessage::computeCrc32(dataTab, getDataSize() - sizeof(unsigned));
    delete[] dataTab;
    return result;
}

bool WifiConfiguration::isValid(void) const
{
    return computeCrc() == crcValue;
}

unsigned WifiConfiguration::getCrc(void) const
{
    return crcValue;
}

void WifiConfiguration::setCrc(void)
{
    crcValue = computeCrc();
}

ISignalPayloadMessage* WifiConfiguration::clone(void) const
{
    return new WifiConfiguration(*this);
}

#ifdef __SKYDIVE_USE_STL__

void WifiConfiguration::setRouterName(const std::string& _routerName)
{
    setRouterName(_routerName.c_str(), _routerName.size());
}

void WifiConfiguration::setRouterPassword(const std::string& _routerPassword)
{
    setRouterPassword(_routerPassword.c_str(), _routerPassword.size());
}

void WifiConfiguration::setRouterIp(const std::string& _routerIp)
{
    setRouterIp(_routerIp.c_str(), _routerIp.size());
}

void WifiConfiguration::setClientIp(const std::string& _clientIp)
{
    setClientIp(_clientIp.c_str(), _clientIp.size());
}

#endif // __SKYDIVE_USE_STL__

void WifiConfiguration::setRouterName(const char* const _routerName, const unsigned size)
{
    if (NULL != routerName)
    {
        delete[] routerName;
    }
    routerName = new char[size];
    routerNameSize = size;
    memcpy(routerName, _routerName, size);
}

void WifiConfiguration::setRouterPassword(const char* const _routerPassword, const unsigned size)
{
    if (NULL != routerPassword)
    {
        delete[] routerPassword;
    }
    routerPassword = new char[size];
    routerPasswordSize = size;
    memcpy(routerPassword, _routerPassword, size);
}

void WifiConfiguration::setRouterIp(const char* const _routerIp, const unsigned size)
{
    if (NULL != routerIp)
    {
        delete[] routerIp;
    }
    routerIp = new char[size];
    routerIpSize = size;
    memcpy(routerIp, _routerIp, size);
}

void WifiConfiguration::setClientIp(const char* const _clientIp, const unsigned size)
{
    if (NULL != clientIp)
    {
        delete[] clientIp;
    }
    clientIp = new char[size];
    clientIpSize = size;
    memcpy(clientIp, _clientIp, size);
}

const char* WifiConfiguration::getRouterName(void) const
{
    return routerName;
}

const char* WifiConfiguration::getRouterPassword(void) const
{
    return routerPassword;
}

const char* WifiConfiguration::getRouterIp(void) const
{
    return routerIp;
}

const char* WifiConfiguration::getClientIp(void) const
{
    return clientIp;
}

unsigned WifiConfiguration::getRouterNameSize(void) const
{
    return routerNameSize;
}

unsigned WifiConfiguration::getRouterPasswordSize(void) const
{
    return routerPasswordSize;
}

unsigned WifiConfiguration::getRouterIpSize(void) const
{
    return routerIpSize;
}

unsigned WifiConfiguration::getClientIpSize(void) const
{
    return clientIpSize;
}

WifiConfiguration& WifiConfiguration::operator=(const WifiConfiguration& right)
{
    setRouterName(right.routerName, right.routerNameSize);
    setRouterPassword(right.routerPassword, right.routerPasswordSize);
    setRouterIp(right.routerIp, right.routerIpSize);
    setClientIp(right.clientIp, right.clientIpSize);
    crcValue = right.crcValue;
    return *this;
}
