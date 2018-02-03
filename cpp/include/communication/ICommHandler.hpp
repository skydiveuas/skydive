// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __I_COMM_HANDLER__
#define __I_COMM_HANDLER__

#include "ICommInterface.hpp"
#include "CommDispatcher.hpp"

#include "CalibrationSettings.hpp"
#include "ControlSettings.hpp"
#include "RouteContainer.hpp"
#include "WifiConfiguration.hpp"

#include "SignalData.hpp"
#include "AutopilotData.hpp"
#include "DebugData.hpp"
#include "ControlData.hpp"
#include "SensorsData.hpp"

/**
 * =============================================================================================
 * main interface to support communications tasks for any peer in SkyDive system
 * all algorithms according to communication protocol and flow are implemented here
 * cause of using this interface is to allow multi platform usage
 * time issues have to be implemented under this interface
 * =============================================================================================
 */
class ICommHandler
{
protected:
    typedef SignalData::Command Command;
    typedef SignalData::Parameter Parameter;

public:
    static const unsigned DEFAULT_TIMEOUT = 1000;
    static const unsigned MAX_RETRANSMISSION = 3;

    ICommHandler(void);
    virtual ~ICommHandler(void);

    // initializer - sets communication interface as operative and resets all helper variables
    void initialize(ICommInterface* _commHandle);

    // resets all buufer in dispather ans used interface
    void reset(void);

    const ICommInterface* getCommHandle(void) const;

    // receiving handler, iteratate through data from commHandle,
    // if data is received returns its preamble type and stops processing received data
    // else return EMPTY type
    IMessage::PreambleType proceedReceiving(void);

    // sending methods
    bool send(const unsigned char* data, const unsigned dataSize);
    bool send(const IMessage& message);
    bool send(const ISignalPayloadMessage& message);

    // methods for receiving specific data from processor buffer
    // when these methods will be called without check for data reception result can be invalid
    SignalData getSignalData(void) const;
    SignalData::Command getCommand(void) const;
    SignalData::Parameter getParameter(void) const;

    DebugData getDebugData(void) const;
    ControlData getControlData(void) const;
    SensorsData getSensorsData(void) const;
    AutopilotData getAutopilotData(void) const;

    void getSignalDataObject(ISignalPayloadMessage& data);

    unsigned getSentMessages(void) const;
    unsigned getReceivedMessage(void) const;
    unsigned getReceptionFailes(void) const;

    void clearCounters(void);

    // commands interface
    bool sendCommand(const SignalData& command);
    bool waitForCommand(const SignalData& command, const unsigned timeout = DEFAULT_TIMEOUT);
    bool waitForAnyCommand(SignalData& command, const unsigned timeout = DEFAULT_TIMEOUT);
    bool waitForAnyParameter(const Command command, Parameter& parameter, const unsigned timeout = DEFAULT_TIMEOUT);
    
    bool sendCommandGetAnyResponse(const SignalData& command, SignalData& response);
    bool sendCommandGetAnyParameter(const SignalData& command, const Command responseCommand, Parameter& response);
    bool sendCommandGetResponse(const SignalData& command, const SignalData& response);
    
    bool readCommandAndRespond(const SignalData& command, const SignalData& response);

    // signal payload data interfaces
    bool sendDataProcedure(const ISignalPayloadMessage& data);
    bool receiveDataProcedure(ISignalPayloadMessage& data);

    virtual void holdThread(const unsigned miliseconds) = 0;

protected:
    virtual void resetTimer(void) = 0;
    virtual unsigned getTimerValue(void) = 0;

private:
    bool sendSignalData(const ISignalPayloadMessage* const data);

    ICommInterface* commInterface;
    CommDispatcher commDispatcher;

    unsigned sentMessages;

    unsigned char* buildingTempBuffer;

#ifdef __SKYDIVE_USE_STL__

public:
    void sendCommandEx(const SignalData& command);
    void waitForCommandEx(const SignalData& command, const unsigned timeout = DEFAULT_TIMEOUT);
    SignalData waitForAnyCommandEx(const unsigned timeout = DEFAULT_TIMEOUT);
    Parameter waitForAnyParameterEx(const Command command, const unsigned timeout = DEFAULT_TIMEOUT);

    SignalData sendCommandGetAnyResponseEx(const SignalData& command);
    Parameter sendCommandGetAnyParameterEx(const SignalData& command, const Command responseCommand);
    void sendCommandGetResponseEx(const SignalData& command, const SignalData& response);

    void readCommandAndRespondEx(const SignalData& command, const SignalData& response);

    void sendDataProcedureEx(const ISignalPayloadMessage& signalPayloadMessage);

    CalibrationSettings receiveCalibarionSettingsEx();
    ControlSettings receiveControlSettingsEx();
    RouteContainer receiveRouteContainerEx();

#endif //__SKYDIVE_USE_STL__
};

#endif // __I_COMM_HANDLER__
