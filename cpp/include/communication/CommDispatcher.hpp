// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __COMM_DISPATCHER__
#define __COMM_DISPATCHER__

#include "IMessage.hpp"

#include "SignalData.hpp"
#include "DebugData.hpp"
#include "SensorsData.hpp"
#include "SignalData.hpp"
#include "AutopilotData.hpp"

#include "ISignalPayloadMessage.hpp"

/**
 * =============================================================================================
 * CommDispatcher
 * =============================================================================================
 */
class CommDispatcher
{
public:
	CommDispatcher(void);
	~CommDispatcher(void);

	// resets all buffers and data
	void reset(void);

	const IMessage::PreambleType& getPreambleType(void) const;

    SignalData::Command getCommand(void) const;
    SignalData::Parameter getParameter(void) const;

    SignalData getSignalData(void) const;
    DebugData getDebugData(void) const;
    ControlData getControlData(void) const;
    SensorsData getSensorsData(void) const;
    AutopilotData getAutopilotData(void) const;

    void getSignalDataObject(ISignalPayloadMessage& data);

    IMessage* retriveMessage(const IMessage::PreambleType preamble,
                             const IMessage::MessageType expectedControlMessageType);
    IMessage* retriveSignalMessage(void);

    unsigned getSucessfullReceptions(void) const;
    unsigned getFailedReceptions(void) const;

	void clearCounters(void);

	IMessage::PreambleType putChar(unsigned char data);

private:
	unsigned char preambleBuffer[IMessage::PREAMBLE_SIZE - 1];
	unsigned char dataBuffer[IMessage::MAX_DATA_SIZE];

	unsigned preambleBufferCounter;
	unsigned dataBufferCounter;

	bool isPreambleActive;
	IMessage::PreambleType activePreambleType;

	unsigned targetDataBufferCounter;

	unsigned failedReceptionCounter;
	unsigned sucessfullReceptionCounter;

	IMessage::PreambleType updatePreamble(unsigned char data);
	void activatePreamble(IMessage::PreambleType preambleType);

	void updataTargetDataSizeWithCommand(void);

	bool receivingSignalData;
	SignalData::Command receivedSignalData;
	unsigned char* signalDataBuffer;
	unsigned signalDataPacketsToReceive;
	unsigned signalDataPacketsReceived;

	void initSignalDataPayloadReception(const SignalData::Command& command, const unsigned short allPackets);
	void handleSignalDataPayloadReception(void);
	bool isSignalDataComplete(void);

    void cleanSignalDataBuffer(void);

	bool isValidMessageCrc(void) const;

	void deactivatePreamble(void);
};

#endif // __COMM_DISPATCHER__
