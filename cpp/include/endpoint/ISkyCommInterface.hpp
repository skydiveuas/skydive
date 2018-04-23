// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef IAPPCOMMINTERFACE_HPP
#define IAPPCOMMINTERFACE_HPP

#include <string>

class ISkyCommInterface
{
public:
    class Listener
    {
    public:
        virtual ~Listener(void);

        virtual void onConnected(void) = 0;

        virtual void onDisconnected(void) = 0;

        virtual void onError(const std::string& message) = 0;

        virtual void onReceived(const unsigned char* data, const size_t length) = 0;
    };

    virtual ~ISkyCommInterface(void);

    virtual void connect(void) = 0;

    virtual void disconnect(void) = 0;

    virtual void send(const unsigned char* data, const size_t length) = 0;

    void setListener(Listener* _listener);

    void onConnected(void);

    void onDisconnected(void);

    void onError(const std::string& message);

    void onReceived(const unsigned char* data, const size_t length);

protected:
    Listener* listener;
};

#endif // IAPPCOMMINTERFACE_HPP
