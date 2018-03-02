// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef IAPPCOMMINTERFACE_HPP
#define IAPPCOMMINTERFACE_HPP

#include <string>

class IAppCommInterface
{
public:
    class Listener
    {
    public:
        virtual ~Listener(void);

        virtual void onConnected(void) = 0;

        virtual void onDisconnected(void) = 0;

        virtual void onError(const std::string& message) = 0;

        virtual void onDataReceived(const unsigned char* data, const unsigned dataSize) = 0;
    };

    virtual ~IAppCommInterface(void);

    virtual void connect(void) = 0;

    virtual void disconnect(void) = 0;

    virtual void sendData(const unsigned char* data, const unsigned dataSize) = 0;

    void setListener(Listener* _listener);

    void onConnected(void);

    void onDisconnected(void);

    void onError(const std::string& message);

    void onDataReceived(const unsigned char* data, const unsigned dataSize);

protected:
    Listener* listener;
};

#endif // IAPPCOMMINTERFACE_HPP
