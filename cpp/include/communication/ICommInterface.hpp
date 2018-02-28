// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __I_COMM_INTERFACE__
#define __I_COMM_INTERFACE__

/**
 * =============================================================================================
 * ICommInterface
 * =============================================================================================
 */
class ICommInterface
{
public:
    /**
     * disconnects and frees communication interface
     */
    virtual void disconnect(void) = 0;

    /**
     * flushes communication interface buffers (both outgoing and incoming)
     */
    virtual void flush(void) = 0;

    /**
     * sets pointer to oldest object in reception buffer and moves cartrige to next element
     * if there is no data in buffer or reception is ongoing - returns false
     * always non-blocking!
     */
    virtual bool getData(unsigned char* data) = 0;

    /**
     * sends data over specific communication interface
     * always non-blocking!
     */
    virtual bool sendData(const unsigned char* data, const unsigned dataCount) = 0;

    /**
     * is communication interface wireless - can be used to fly
     */
    virtual bool isWireless(void) const = 0;

    /**
     * is communication interface connected
     */
    virtual bool isConnected(void) const
    {
        return connected;
    }

    ICommInterface(void)
    {
        connected = false;
    }

    /**
     * virtual destructor for memmory safeness
     */
    virtual ~ICommInterface(void)
    {
        // nothing to do here
    }

protected:
    volatile bool connected;
};

#endif // __I_COMM_INTERFACE__
