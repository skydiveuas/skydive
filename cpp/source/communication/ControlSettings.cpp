#include "ControlSettings.hpp"

#ifdef __SKYDIVE_USE_STL__

#include "SkyException.hpp"

#endif // __SKYDIVE_USE_STL__

#include <string.h>

#include "IMessage.hpp"
#include "SignalData.hpp"
#include "DebugData.hpp"

ControlSettings::ControlSettings(void)
{
}

ControlSettings::ControlSettings(const unsigned char* src)
{
    memcpy((unsigned char*)this + 4, src, getDataSize());
}

void ControlSettings::serialize(unsigned char* dst) const
{
    memcpy(dst, (unsigned char*)this + 4, getDataSize());
}

unsigned ControlSettings::getDataSize(void) const
{
    return sizeof(ControlSettings) - 4;
}

SignalData::Command ControlSettings::getSignalDataType(void) const
{
    return SignalData::CONTROL_SETTINGS_DATA;
}

SignalData::Command ControlSettings::getSignalDataCommand(void) const
{
    return SignalData::CONTROL_SETTINGS;
}

SignalData::Command ControlSettings::getUploadAction(void) const
{
    return SignalData::UPLOAD_SETTINGS;
}

IMessage::MessageType ControlSettings::getMessageType(void) const
{
    return CONTROL_SETTINGS;
}

bool ControlSettings::isValid(void) const
{
    switch (uavType)
    {
    case TRICOPTER_REAR:
    case TRICOPTER_FRONT:
    case QUADROCOPTER_X:
    case QUADROCOPTER_PLUS:
    case HEXACOPTER_X:
    case HEXACOPTER_PLUS:
    case OCTOCOPTER_X:
    case OCTOCOPTER_PLUS:
        break;
    default:
        return false;
    }

    switch (initialSolverMode)
    {
    case ControlData::STABLILIZATION:
    case ControlData::ANGLE_NO_YAW:
    case ControlData::ANGLE:
    case ControlData::HEADLESS:
        break;
    default:
        return false;
    }

    switch (manualThrottleMode)
    {
    case STATIC:
    case DYNAMIC:
        break;
    default:
        return false;
    }

    switch (stickMovementMode)
    {
    case COPTER:
    case GEOGRAPHIC:
    case BASE_POINT:
        break;
    default:
        return false;
    }

    switch (batteryType)
    {
    case UNDEFINED:
    case BATTERY_2S:
    case BATTERY_3S:
    case BATTERY_4S:
    case BATTERY_5S:
    case BATTERY_6S:
        break;
    default:
        return false;
    }

    switch (errorHandlingAction)
    {
    case DebugData::AUTOLANDING:
    case DebugData::AUTOLANDING_AP:
    case DebugData::BACK_TO_BASE:
        break;
    default:
        return false;
    }

    if (autoLandingDescedRate < 0.0f)
    {
        return false;
    }

    unsigned char dataTab[sizeof(ControlSettings)];
    serialize(dataTab);
    return IMessage::computeCrc32(dataTab, getDataSize() - 4) == crcValue;
}

unsigned ControlSettings::getCrc(void) const
{
    return crcValue;
}

void ControlSettings::setCrc(void)
{
    unsigned char dataTab[sizeof(ControlSettings)];
    serialize(dataTab);
    crcValue = IMessage::computeCrc32(dataTab, getDataSize() - 4);
}

ISignalPayloadMessage* ControlSettings::clone(void) const
{
    return new ControlSettings(*this);
}

ControlSettings::UavType ControlSettings::getUavType() const
{
    return static_cast<UavType>(uavType);
}

ControlData::SolverMode ControlSettings::getSolverMode() const
{
    return static_cast<ControlData::SolverMode>(initialSolverMode);
}

ControlSettings::ThrottleMode ControlSettings::getManualThrottleMode() const
{
    return static_cast<ThrottleMode>(manualThrottleMode);
}

ControlSettings::StickMovementMode ControlSettings::getStickMovementMode() const
{
    return static_cast<StickMovementMode>(stickMovementMode);
}

ControlSettings::BatteryType ControlSettings::getBatteryType() const
{
    return static_cast<BatteryType>(batteryType);
}

ControlData::ControllerCommand ControlSettings::getErrorHandlingAction() const
{
    return static_cast<ControlData::ControllerCommand>(uavType);
}

ControlSettings::EscPwmFreq ControlSettings::getEscPwmFreq() const
{
    return static_cast<EscPwmFreq>(escPwmFreq);
}

float ControlSettings::getBatteryErrorLevel(void) const
{
    return (float)batteryType * 3.5f;
}

float ControlSettings::getBatteryWarningLevel(void) const
{
    return (float)batteryType * 3.7f;
}

float ControlSettings::getBatteryMaxLevel(void) const
{
    return (float)batteryType * 4.2f;
}

float ControlSettings::getBatteryPercentage(const float voltage) const
{
    if (batteryType != UNDEFINED && voltage > 5.0f)
    {
        const float min = getBatteryErrorLevel();
        const float max = getBatteryMaxLevel() - min;
        const float v = voltage - min;
        const float result = (v * 100.0f) / max;
        return roboLib::minmaxVal(0.0f, 100.0f, result);
    }
    else
    {
        return 0.0f;
    }
}

bool ControlSettings::isBatteryError(const float voltage) const
{
    if (batteryType != UNDEFINED && voltage > 5.0f)
    {
        return voltage < getBatteryErrorLevel();
    }
    else
    {
        return false;
    }
}

bool ControlSettings::isBatteryWarning(const float voltage) const
{
    if (batteryType != UNDEFINED && voltage > 5.0f)
    {
        return voltage < getBatteryWarningLevel();
    }
    else
    {
        return false;
    }
}

ControlData::SolverMode ControlSettings::getInitialSolverMode(void) const
{
    return (ControlData::SolverMode)initialSolverMode;
}

ControlData ControlSettings::formatEulers(const ControlData& controlData) const
{
    ControlData result(controlData);
    Vect3Df euler(controlData.getEuler());
    euler.x *= maxRollPitchControlValue;
    euler.y *= maxRollPitchControlValue;
    euler.z *= maxYawControlValue;
    result.setEuler(euler);
    return result;
}

ControlSettings ControlSettings::createDefault(void)
{
    ControlSettings defaultControlSettings;
    defaultControlSettings.rollProp = 0.0f;
    defaultControlSettings.pitchProp = 0.0f;
    defaultControlSettings.yawProp = 0.0f;
    defaultControlSettings.autoLandingDescedRate = 0.0f;
    defaultControlSettings.maxAutoLandingTime = 0.0f;
    defaultControlSettings.maxRollPitchControlValue = 0.0f;
    defaultControlSettings.maxYawControlValue = 0.0f;

    // throttle controller
    defaultControlSettings.maxVerticalAutoVelocity = 0.0f;
    defaultControlSettings.altPositionProp = 0.0f;
    defaultControlSettings.altVelocityProp = 0.0f;
    defaultControlSettings.throttleAltRateProp = 0.0f;

    // autopilot
    defaultControlSettings.autoPositionProp = 0.0f;
    defaultControlSettings.autoVelocityProp = 0.0f;
    defaultControlSettings.maxAutoAngle = 0.0f;
    defaultControlSettings.maxAutoVelocity = 0.0f;
    defaultControlSettings.stickPositionRateProp = 0.0f;
    defaultControlSettings.stickMovementMode = COPTER;

    defaultControlSettings.batteryType = UNDEFINED;

    defaultControlSettings.errorHandlingAction = DebugData::AUTOLANDING;

    defaultControlSettings.escPwmFreq = MEDIUM;

    defaultControlSettings.gpsSensorPosition = Vect3Df();

    defaultControlSettings.flags = 0;

    return defaultControlSettings;
}

bool ControlSettings::isUavBisymetricalFromData(void) const
{
    return (rollProp == pitchProp && pidRollRate == pidPitchRate);
}

ControlData ControlSettings::getControlDataForLogs(const ControlData& controlData) const
{
    ControlData result(controlData);
    Vect3Df resultEuler = result.getEuler();
    resultEuler.x *= maxRollPitchControlValue;
    resultEuler.y *= maxRollPitchControlValue;
    resultEuler.z *= maxYawControlValue;
    result.setEuler(resultEuler);
    return result;
}

float ControlSettings::retriveAngleTuner(void)
{
    return 0.5f;
}

float ControlSettings::retriveRateTuner(void)
{
    return 0.5f;
}

float ControlSettings::getTunerRpProp(const float tuner)
{
    return 6.0f*tuner + 3.0f;
}

Vect3Df ControlSettings::getTunerRpRate(const float tuner)
{
    const float tunerPow = tuner*tuner;
    const float p = 0.69333f*tunerPow + 0.08667f;
    return Vect3Df(
                p,
                p * 0.6f,
                0.03200f*tunerPow + 0.00400f);
}

float ControlSettings::getTunerYawProp(const float tuner)
{
    return 6.0f*tuner + 3.0f;
}

Vect3Df ControlSettings::getTunerYawRate(const float tuner)
{
    const float tunerPow = tuner*tuner;
    return Vect3Df(
                0.69333f*tunerPow + 0.08667f,
                0.34667f*tunerPow + 0.43333f,
                0.03200f*tunerPow + 0.00400f);
}

Vect2Df ControlSettings::retriveAutopilotTuners(void)
{
    return Vect2Df(
                0.5f,
                0.5f);
}

Vect2Df ControlSettings::getTunerVerticalPropSpeed(const float speed, const float force)
{
    return Vect2Df(
                3.21f*speed*force + 4.2f,
                2.31f*speed*force + 2.1f);
}

Vect3Df ControlSettings::getTunerVerticalAccel(const float speed, const float force)
{
    return Vect3Df(
                0.9333f*speed*force + 0.0867f,
                0.34667f*speed*force + 0.4333f,
                0.320f*speed*force + 0.040f);
}

Vect2Df ControlSettings::getTunerHorizontalPropSpeed(const float speed, const float force)
{
    return Vect2Df(
                1.21f*speed*force + 2.2f,
                3.31f*speed*force + 2.1f);
}

Vect3Df ControlSettings::getTunerHorizontalAccel(const float speed, const float force)
{
    return Vect3Df(
                0.2933f*speed*force + 0.0867f,
                0.767f*speed*force + 0.4333f,
                0.9320f*speed*force + 0.1f);
}

#ifdef __SKYDIVE_USE_STL__

std::string ControlSettings::getUavTypeString(UavType type)
{
    switch (type)
    {
    case ControlSettings::TRICOPTER_REAR:
        return std::string("Tricopter rear");
    case ControlSettings::TRICOPTER_FRONT:
        return std::string("Tricopter front");
    case ControlSettings::QUADROCOPTER_X:
        return std::string("Quadrocopter \"X\"");
    case ControlSettings::QUADROCOPTER_PLUS:
        return std::string("Quadrocopter \"+\"");
    case ControlSettings::HEXACOPTER_X:
        return std::string("Hexacopter \"X\"");
    case ControlSettings::HEXACOPTER_PLUS:
        return std::string("Hexacopter \"+\"");
    case ControlSettings::OCTOCOPTER_X:
        return std::string("Octocopter \"X\"");
    case ControlSettings::OCTOCOPTER_PLUS:
        return std::string("Octocopter \"+\"");
    default:
        __SKY_EXCEPTION__("Unexpeted uav type");
    }
}

std::string ControlSettings::getUavTypeString(void) const
{
    return getUavTypeString(static_cast<UavType>(uavType));
}
std::string ControlSettings::getInitialSolverString(void) const
{
    return ControlData::getSolverModeString(
                static_cast<ControlData::SolverMode>(initialSolverMode));
}

std::string ControlSettings::getManualThrottleString(void) const
{
    switch (manualThrottleMode)
    {
    case ControlSettings::STATIC:
        return std::string("Static");
    case ControlSettings::DYNAMIC:
        return std::string("Dynamic");
    default:
        return std::string("Error");
    }
}

std::string ControlSettings::getStickModeString(void) const
{
    switch (stickMovementMode)
    {
    case ControlSettings::COPTER:
        return std::string("Copter");
    case ControlSettings::GEOGRAPHIC:
        return std::string("Geographic");
    case ControlSettings::BASE_POINT:
        return std::string("Base point");
    default:
        return std::string("Error");
    }
}

std::string ControlSettings::getBatteryTypeString(void) const
{
    if (batteryType != ControlSettings::UNDEFINED)
    {
        return std::to_string((long long)batteryType) + "S";
    }
    else
    {
        return "Disabled";
    }
}

std::string ControlSettings::getErrorHandlingActionString(void) const
{
    return ControlData::getControllerCommandString(
                (ControlData::ControllerCommand)errorHandlingAction);
}

std::string ControlSettings::getEscPwmFreqString(void) const
{
    switch (escPwmFreq)
    {
    case ControlSettings::SLOW:
        return std::string("Slow");
    case ControlSettings::MEDIUM:
        return std::string("Medium");
    case ControlSettings::FAST:
        return std::string("Fast");
    case ControlSettings::VERY_FAST:
        return std::string("Very fast");
    case ControlSettings::ONESHOT_125:
        return std::string("Oneshot125");
    default:
        return std::string("Error");
    }
}

ControlSettings::UavType ControlSettings::uavTypeFromString(const std::string& uavTypeString)
{
    if (uavTypeString == "Tricopter rear") return ControlSettings::TRICOPTER_REAR;
    else if (uavTypeString == "Tricopter front") return ControlSettings::TRICOPTER_FRONT;
    else if (uavTypeString == "Quadrocopter \"X\"") return ControlSettings::QUADROCOPTER_X;
    else if (uavTypeString == "Quadrocopter \"+\"") return ControlSettings::QUADROCOPTER_PLUS;
    else if (uavTypeString == "Hexacopter \"X\"") return ControlSettings::HEXACOPTER_X;
    else if (uavTypeString == "Hexacopter \"+\"") return ControlSettings::HEXACOPTER_PLUS;
    else if (uavTypeString == "Octocopter \"X\"") return ControlSettings::OCTOCOPTER_X;
    else if (uavTypeString == "Octocopter \"+\"") return ControlSettings::OCTOCOPTER_PLUS;
    else __SKY_EXCEPTION__("Incorrect string for UavType");
}

ControlData::SolverMode ControlSettings::solverModeFromString(const std::string& solverMode)
{
    if (solverMode == "Stabilization") return ControlData::STABLILIZATION;
    else if (solverMode == "Angle") return ControlData::ANGLE;
    else if (solverMode == "Angle no yaw") return ControlData::ANGLE_NO_YAW;
    else if (solverMode == "Headless") return ControlData::HEADLESS;
    else __SKY_EXCEPTION__("Incorrect string for InitialSolverMode");
}

ControlSettings::ThrottleMode ControlSettings::manualThrottleModeFromString(const std::string& throttleMode)
{
    if (throttleMode == "Static") return ControlSettings::STATIC;
    else if (throttleMode == "Dynamic") return ControlSettings::DYNAMIC;
    else __SKY_EXCEPTION__("Incorrect string for MaunalThrottleMode");
}

ControlSettings::StickMovementMode ControlSettings::stickModeFromString(const std::string& stickMode)
{
    if (stickMode == "Copter") return ControlSettings::COPTER;
    else if (stickMode == "Geographic") return ControlSettings::GEOGRAPHIC;
    else if (stickMode == "Base point") return ControlSettings::BASE_POINT;
    else __SKY_EXCEPTION__("Incorrect string for StickMode");
}

ControlSettings::BatteryType ControlSettings::batteryTypeFromString(const std::string& batteryType)
{
    if (batteryType == "Disabled")
    {
        return ControlSettings::UNDEFINED;
    }
    else
    {
        const char str = batteryType.c_str()[0];
        return (ControlSettings::BatteryType)atoi(&str);
    }
}

ControlData::ControllerCommand ControlSettings::errorHandlingActionFromString(const std::string& string)
{
    if (string == ControlData::getControllerCommandString(ControlData::AUTOLANDING))
        return ControlData::AUTOLANDING;
    else if (string == ControlData::getControllerCommandString(ControlData::AUTOLANDING_AP))
        return ControlData::AUTOLANDING_AP;
    else if (string == ControlData::getControllerCommandString(ControlData::BACK_TO_BASE))
        return ControlData::BACK_TO_BASE;
    else
        __SKY_EXCEPTION__("Incorrect string for ErrorHandlingAction");
}

ControlSettings::EscPwmFreq ControlSettings::escPwmFreqFromString(const std::string& string)
{
    if (string == "Slow") return ControlSettings::SLOW;
    else if (string == "Medium") return ControlSettings::MEDIUM;
    else if (string == "Fast") return ControlSettings::FAST;
    else if (string == "Very fast") return ControlSettings::VERY_FAST;
    else if (string == "Oneshot125") return ControlSettings::ONESHOT_125;
    else __SKY_EXCEPTION__("Incorrect string for EscPwmFreq");
}

std::vector<std::string> ControlSettings::getUavTypes(void)
{
    return std::vector<std::string> {
        "Tricopter rear",
        "Tricopter front",
        "Quadrocopter \"X\"",
        "Quadrocopter \"+\"",
        "Hexacopter \"X\"",
        "Hexacopter \"+\"",
        "Octocopter \"X\"",
        "Octocopter \"+\""
    };
}

std::vector<std::string> ControlSettings::getSolvers(void)
{
    return std::vector<std::string> {
        "Stabilization",
        "Angle",
        "Angle no yaw",
        "Headless"
    };
}

std::vector<std::string> ControlSettings::getThrottleModes(void)
{
    return std::vector<std::string> {
        "Static",
        "Dynamic"
    };
}

std::vector<std::string> ControlSettings::getStickModes(void)
{
    return std::vector<std::string> {
        "Copter",
        "Geographic",
        "Base point"
    };
}

std::vector<std::string> ControlSettings::getBatteryTypes(void)
{
    return std::vector<std::string> {
        "Disabled",
        "2S",
        "3S",
        "4S",
        "5S",
        "6S"
    };
}

std::vector<std::string> ControlSettings::getErrorHandlingActions(void)
{
    return std::vector<std::string> {
        ControlData::getControllerCommandString(ControlData::AUTOLANDING),
                ControlData::getControllerCommandString(ControlData::AUTOLANDING_AP),
                ControlData::getControllerCommandString(ControlData::BACK_TO_BASE)
    };
}

std::vector<std::string> ControlSettings::getEscPwmFreqs(void)
{
    return std::vector<std::string> {
        "Slow",
        "Medium",
        "Fast",
        "Very fast",
        "Oneshot125"
    };
}

std::ostream& operator << (std::ostream& stream, const ControlSettings& cS)
{
    stream << "CONTROL SETTINGS: " << std::endl;
    stream << "roll:   " << cS.pidRollRate.x << "	" << cS.pidRollRate.y << "	" << cS.pidRollRate.z  << std::endl;
    stream << "pitch:  " << cS.pidPitchRate.x << "	" << cS.pidPitchRate.y << "	" << cS.pidPitchRate.z << std::endl;
    stream << "yaw:    " << cS.pidYawRate.x << "	" << cS.pidYawRate.y << "	" << cS.pidYawRate.z << std::endl;
    return stream;
}

#endif // __SKYDIVE_USE_STL__
