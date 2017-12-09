// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __I_MESSAGE__
#define __I_MESSAGE__

#ifdef __SKYDIVE_USE_STL__

#include <string>

#endif // __SKYDIVE_USE_STL__

/**
 * =============================================================================================
 * IMessage
 * =============================================================================================
 */
class IMessage
{
public:
    enum PreambleType
	{
		EMPTY,
		CONTROL,
		SIGNAL,
		AUTOPILOT
	};

    enum MessageType
    {
        DEBUG_DATA,
        CONTROL_DATA,
        SENSORS_DATA,
        AUTOPILOT_DATA,
        SIGNAL_DATA,
        CALIBRATION_SETTINGS,
        CONTROL_SETTINGS,
        ROUTE_CONTAINER,
        WIFI_CONFIGURATION
    };

	virtual unsigned getPayloadSize(void) const;
	virtual unsigned getMessageSize(void) const;

	virtual PreambleType getPreambleType(void) const = 0;

	virtual void serialize(unsigned char* data) const = 0;
	virtual void serializeMessage(unsigned char* data) const;

    virtual MessageType getMessageType(void) const = 0;

	// creates dynamically allocated message array with binary communication data
	unsigned char* createMessage(void) const;

    virtual bool isSignalPayloadMessage(void) const;

	// virtual destructor for memory safeness
	virtual ~IMessage(void);

	static unsigned char getPreambleCharByType(const PreambleType type);
	static PreambleType getPreabmleTypeByChar(const unsigned char data);

	static unsigned getPayloadSizeByType(const PreambleType type);

	static unsigned short computeCrc16(const unsigned char* data, const unsigned dataSize);
	static unsigned computeCrc32(const unsigned char* data, const unsigned dataSize);

	static const unsigned PREAMBLE_SIZE = 4;
	static const unsigned SIGNAL_COMMAND_SIZE = 4;
	static const unsigned SIGNAL_CONSTRAINT_SIZE = 8;
	static const unsigned SIGNAL_DATA_PAYLOAD_SIZE = 50;
	static const unsigned CRC_SIZE = 2;

	static const unsigned SIGNAL_COMMAND_MESSAGE_SIZE =
			PREAMBLE_SIZE + SIGNAL_CONSTRAINT_SIZE + CRC_SIZE;
	static const unsigned SIGNAL_DATA_MESSAGE_SIZE =
			PREAMBLE_SIZE + SIGNAL_CONSTRAINT_SIZE + SIGNAL_DATA_PAYLOAD_SIZE + CRC_SIZE;
	static const unsigned MAX_DATA_SIZE = SIGNAL_DATA_MESSAGE_SIZE;

#ifdef __SKYDIVE_USE_STL__

    virtual std::string toString(void) const;

    std::string getMessageName(void) const;

    static std::string toString(const MessageType type);

#endif // __SKYDIVE_USE_STL__
};

#endif // __I_MESSAGE__
