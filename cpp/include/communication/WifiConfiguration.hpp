// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __WIFI_CONFIGURATION__
#define __WIFI_CONFIGURATION__

#include "ISignalPayloadMessage.hpp"

/**
 * =============================================================================================
 * WifiConfiguration
 * =============================================================================================
 */
class WifiConfiguration : public ISignalPayloadMessage
{
public:
    WifiConfiguration(void);
	WifiConfiguration(const WifiConfiguration& wifiConfiguration);
    WifiConfiguration(const unsigned char* const src);

    ~WifiConfiguration(void);

    void serialize(unsigned char* dst) const;

    unsigned getDataSize(void) const;

    SignalData::Command getSignalDataType(void) const;
    SignalData::Command getSignalDataCommand(void) const;
    SignalData::Command getUploadAction(void) const;

    MessageType getMessageType(void) const;

    unsigned computeCrc() const;

    bool isValid(void) const;

    unsigned getCrc(void) const;

    void setCrc(void);

    ISignalPayloadMessage* clone(void) const;

#ifdef __SKYDIVE_USE_STL__

    void setRouterName(const std::string& _routerName);
    void setRouterPassword(const std::string& _routerPassword);
    void setRouterIp(const std::string& _routerIp);
    void setClientIp(const std::string& _clientIp);

#endif // __SKYDIVE_USE_STL__

    void setRouterName(const char* const _routerName, const unsigned size);
    void setRouterPassword(const char* const _routerPassword, const unsigned size);
    void setRouterIp(const char* const _routerIp, const unsigned size);
    void setClientIp(const char* const _clientIp, const unsigned size);

    const char* getRouterName(void) const;
    const char* getRouterPassword(void) const;
    const char* getRouterIp(void) const;
    const char* getClientIp(void) const;

    unsigned getRouterNameSize(void) const;
    unsigned getRouterPasswordSize(void) const;
    unsigned getRouterIpSize(void) const;
    unsigned getClientIpSize(void) const;

    WifiConfiguration& operator=(const WifiConfiguration& right);

private:
    static const unsigned SIZES_VALUES_SIZE = 4 * sizeof(unsigned);

    unsigned routerNameSize;
    unsigned routerPasswordSize;
    unsigned routerIpSize;
    unsigned clientIpSize;

    char* routerName;
    char* routerPassword;
    char* routerIp;
    char* clientIp;

    unsigned crcValue;
};

#endif // __WIFI_CONFIGURATION__
