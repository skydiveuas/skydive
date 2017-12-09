// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __CONTROL_SETTINGS__
#define __CONTROL_SETTINGS__

#ifdef __SKYDIVE_USE_STL__

#include <string>
#include <vector>

#endif // __SKYDIVE_USE_STL__

#include "MathCore.hpp"

#include "ISignalPayloadMessage.hpp"

#include "SignalData.hpp"
#include "ControlData.hpp"
#include "Flags.hpp"

/**
 * =============================================================================================
 * ControlSettings
 * =============================================================================================
 */
class ControlSettings : public ISignalPayloadMessage
{
public:
	enum UavType
	{
        TRICOPTER_REAR = 1000,
        TRICOPTER_FRONT = 1500,
		QUADROCOPTER_X = 2000,
		QUADROCOPTER_PLUS = 2500,
		HEXACOPTER_X = 3000,
		HEXACOPTER_PLUS = 3500,
		OCTOCOPTER_X = 4000,
		OCTOCOPTER_PLUS = 4500
	};

	enum ThrottleMode // unsigned char
	{
		STATIC = 10,
		DYNAMIC = 20,
		RATE = 30,
		ALTITUDE = 40
	};

	enum StickMovementMode
	{
		COPTER,
		GEOGRAPHIC,
		BASE_POINT
	};

	enum BatteryType
	{
		UNDEFINED = 0,
		BATTERY_2S = 2,
		BATTERY_3S,
		BATTERY_4S,
		BATTERY_5S,
		BATTERY_6S
	};

	enum EscPwmFreq
	{
		SLOW = 100,
		MEDIUM = 200,
		FAST = 300,
		VERY_FAST = 400,
		ONESHOT_125 = 3200
	};

	enum FlagId
	{
		ENABLE_FLIGHT_LOGGER,
		ALLOW_DYNAMIC_AUTOPILOT,
        EMPTY, // to be used by new flag
        ANGLES_TUNED,
        AUTOPILOT_TUNED
	};

	// base control settings
	int uavType;
	int initialSolverMode;
	int manualThrottleMode;

	// auto landing settings
	float autoLandingDescedRate;
	float maxAutoLandingTime;

	// control values
	float maxRollPitchControlValue;
	float maxYawControlValue;

	// regulator control settings
	// stabilization
	Vect3Df pidRollRate, pidPitchRate, pidYawRate;
	// angle
	float rollProp, pitchProp, yawProp;

	// throttle controller settings
    float maxVerticalAutoVelocity;
	float altPositionProp;
	float altVelocityProp;
	Vect3Df pidThrottleAccel;

	// throttle controller stick settings
	float throttleAltRateProp;

	// autopilot settings
	float maxAutoAngle;
	float maxAutoVelocity;
	float autoPositionProp;
	float autoVelocityProp;
	Vect3Df pidAutoAccel;

	// autopilot stick settings
	float stickPositionRateProp;
	int stickMovementMode;

	// battery type
	int batteryType;

	// error handling type
	int errorHandlingAction;

	// esc controller PWM frequency
	int escPwmFreq;

	// gps sensors position in rigid body coordinate system
	Vect3Df gpsSensorPosition;

	// flags for any boolean settings
    Flags<unsigned> flags;

	ControlSettings(void);
	ControlSettings(const unsigned char* src);

	void serialize(unsigned char* dst) const;

	unsigned getDataSize() const;

	SignalData::Command getSignalDataType(void) const;
	SignalData::Command getSignalDataCommand(void) const;
    SignalData::Command getUploadAction(void) const;

    MessageType getMessageType(void) const;

	bool isValid(void) const;

    unsigned getCrc(void) const;

	void setCrc(void);

    ISignalPayloadMessage* clone(void) const;

    UavType getUavType() const;
    ControlData::SolverMode getSolverMode() const;
    ThrottleMode getManualThrottleMode() const;
    StickMovementMode getStickMovementMode() const;
    BatteryType getBatteryType() const;
    ControlData::ControllerCommand getErrorHandlingAction() const;
    EscPwmFreq getEscPwmFreq() const;

	float getBatteryErrorLevel(void) const;
	float getBatteryWarningLevel(void) const;
	float getBatteryMaxLevel(void) const;
    float getBatteryPercentage(const float voltage) const;

	bool isBatteryError(const float voltage) const;
	bool isBatteryWarning(const float voltage) const;

	ControlData::SolverMode getInitialSolverMode(void) const;

	ControlData formatEulers(const ControlData& controlData) const;

	static ControlSettings createDefault(void);

	bool isUavBisymetricalFromData(void) const;

	ControlData getControlDataForLogs(const ControlData& controlData) const;

    float retriveAngleTuner(void);
    float retriveRateTuner(void);

    static float getTunerRpProp(const float tuner);
    static Vect3Df getTunerRpRate(const float tuner);
    static float getTunerYawProp(const float tuner);
    static Vect3Df getTunerYawRate(const float tuner);

    Vect2Df retriveAutopilotTuners(void);

    static Vect2Df getTunerVerticalPropSpeed(const float speed, const float force);
    static Vect3Df getTunerVerticalAccel(const float speed, const float force);
    static Vect2Df getTunerHorizontalPropSpeed(const float speed, const float force);
    static Vect3Df getTunerHorizontalAccel(const float speed, const float force);

#ifdef __SKYDIVE_USE_STL__

    static std::string getUavTypeString(UavType type);

    std::string getUavTypeString(void) const;
    std::string getInitialSolverString(void) const;
    std::string getManualThrottleString(void) const;
    std::string getStickModeString(void) const;
    std::string getBatteryTypeString(void) const;
    std::string getErrorHandlingActionString(void) const;
    std::string getEscPwmFreqString(void) const;

    static UavType uavTypeFromString(const std::string& uavTypeString);
    static ControlData::SolverMode solverModeFromString(const std::string& solverMode);
    static ThrottleMode manualThrottleModeFromString(const std::string& throttleMode);
    static StickMovementMode stickModeFromString(const std::string& stickMode);
    static BatteryType batteryTypeFromString(const std::string& batteryType);
    static ControlData::ControllerCommand errorHandlingActionFromString(const std::string& errorHandlingAction);
    static EscPwmFreq escPwmFreqFromString(const std::string& escPwmFreq);

    static std::vector<std::string> getUavTypes(void);
    static std::vector<std::string> getSolvers(void);
    static std::vector<std::string> getThrottleModes(void);
    static std::vector<std::string> getStickModes(void);
    static std::vector<std::string> getBatteryTypes(void);
    static std::vector<std::string> getErrorHandlingActions(void);
    static std::vector<std::string> getEscPwmFreqs(void);

    friend std::ostream& operator << (std::ostream& stream, const ControlSettings& cS);

#endif // __SKYDIVE_USE_STL__

private:
	unsigned crcValue;
};

#endif // __CONTROL_SETTINGS__
