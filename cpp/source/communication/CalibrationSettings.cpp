#include "communication/CalibrationSettings.hpp"

#ifdef __SKYDIVE_USE_STL__

#include <ostream>

#endif // __SKYDIVE_USE_STL__

#include <string.h>

#include "communication/IMessage.hpp"
#include "communication/SignalData.hpp"

CalibrationSettings::CalibrationSettings(void)
{
}

CalibrationSettings::CalibrationSettings(const unsigned char* src)
{
    memcpy((unsigned char*)this + 4, src, getDataSize());
}

void CalibrationSettings::serialize(unsigned char* dst) const
{
    memcpy(dst, (unsigned char*)this + 4, getDataSize());
}

unsigned CalibrationSettings::getDataSize(void) const
{
    return sizeof(CalibrationSettings) - 4;
}

SignalData::Command CalibrationSettings::getSignalDataType(void) const
{
    return SignalData::CALIBRATION_SETTINGS_DATA;
}

SignalData::Command CalibrationSettings::getSignalDataCommand(void) const
{
    return SignalData::CALIBRATION_SETTINGS;
}

SignalData::Command CalibrationSettings::getUploadAction(void) const
{
    // calibration settings are never uploaded to board!
    return SignalData::DUMMY;
}

IMessage::MessageType CalibrationSettings::getMessageType(void) const
{
    return CALIBRATION_SETTINGS;
}

CalibrationSettings CalibrationSettings::createDefault(void)
{
    CalibrationSettings calibData;
    calibData.gyroOffset = Vect3Df();
    calibData.accelCalib = Mat3Df::eye();
    calibData.magnetSoft = Mat3Df::eye();
    calibData.magnetHard = Vect3Df();
    calibData.altimeterSetting = (float)roboLib::normPressure;
    calibData.temperatureSetting = (float)roboLib::normTemperature;
    calibData.radioLevels = getDefaultRadioLevels();
    calibData.setDefaultPwmInputMap();
    calibData.boardType = TYPE_UNKNOWN;
    calibData.flags = 0;
    calibData.setCrc();
    return calibData;
}

bool CalibrationSettings::isValid(void) const
{
    switch (boardType)
    {
    case TYPE_UNKNOWN:
    case TYPE_ULTIMATE_V5:
    case TYPE_BASIC_V2:
    case TYPE_BASIC_V3:
    case TYPE_PRO_V1:
        break;
    default:
        return false;
    }

    unsigned char dataTab[sizeof(CalibrationSettings)];
    serialize(dataTab);
    return IMessage::computeCrc32(dataTab, getDataSize() - 4) == crcValue
            && magnetSoft.getDet() != 0.0f
            && accelCalib.getDet() != 0.0f;
}

unsigned CalibrationSettings::getCrc(void) const
{
    return crcValue;
}

void CalibrationSettings::setCrc(void)
{
    unsigned char dataTab[sizeof(CalibrationSettings)];
    serialize(dataTab);
    crcValue = IMessage::computeCrc32(dataTab, getDataSize() - 4);
}

ISignalPayloadMessage* CalibrationSettings::clone(void) const
{
    return new CalibrationSettings(*this);
}

CalibrationSettings::BoardType CalibrationSettings::getBoardType() const
{
    return static_cast<BoardType>(boardType);
}

Mat4Df CalibrationSettings::getDefaultRadioLevels(void)
{
    Mat4Df inputLevels;

    inputLevels.mat[0] = 1.0f;
    inputLevels.mat[1] = 2.0f;

    inputLevels.mat[2] = 1.0f;
    inputLevels.mat[3] = 2.0f;

    inputLevels.mat[4] = 1.0f;
    inputLevels.mat[5] = 2.0f;

    inputLevels.mat[6] = 1.0f;
    inputLevels.mat[7] = 2.0f;

    inputLevels.mat[8] = 1.0f;
    inputLevels.mat[9] = 2.0f;

    inputLevels.mat[10] = 1.0f;
    inputLevels.mat[11] = 2.0f;

    inputLevels.mat[12] = 1.0f;
    inputLevels.mat[13] = 2.0f;

    inputLevels.mat[14] = 1.0f;
    inputLevels.mat[15] = 2.0f;

    return inputLevels;
}

void CalibrationSettings::setDefaultPwmInputMap(void)
{
    for (unsigned char i = 0; i<8; i++)
    {
        this->pwmInputMapData[i] = -1;
    }
}

void CalibrationSettings::setPwmInputMap(char* map)
{
    for (unsigned i = 0; i < 8; i++)
    {
        this->pwmInputMapData[i] = map[i];
    }
}

#ifdef __SKYDIVE_USE_STL__

std::string CalibrationSettings::getBoardTypeString(BoardType boardType)
{
    switch (boardType)
    {
    case CalibrationSettings::TYPE_ULTIMATE_V5:
        return "ULTIMATE v5";
    case CalibrationSettings::TYPE_BASIC_V2:
        return "BASIC v2";
    case CalibrationSettings::TYPE_BASIC_V3:
        return "BASIC v3";
    case CalibrationSettings::TYPE_PRO_V1:
        return "PRO v1";
    default:
        return "Unknown board type";
    }
}

std::string CalibrationSettings::getBoardTypeString(void) const
{
    return getBoardTypeString(static_cast<BoardType>(boardType));
}

CalibrationSettings::BoardType CalibrationSettings::boardTypeFromString(const std::string& boardTypeString)
{
	if (boardTypeString == "ULTIMATE v5") return CalibrationSettings::TYPE_ULTIMATE_V5;
    else if (boardTypeString == "BASIC v2") return CalibrationSettings::TYPE_BASIC_V2;
    else if (boardTypeString == "BASIC v3") return CalibrationSettings::TYPE_BASIC_V3;
    else if (boardTypeString == "PRO v1") return CalibrationSettings::TYPE_PRO_V1;
    else return CalibrationSettings::TYPE_UNKNOWN;
}

std::ostream& operator << (std::ostream& stream, const CalibrationSettings& cD)
{
    stream << "gyro offset:	" << cD.gyroOffset.x << ", " << cD.gyroOffset.y << ", " << cD.gyroOffset.z << std::endl << std::endl;

    stream << "preasure:	" << cD.altimeterSetting << " hPa" << std::endl;
    stream << "temperature:	" << cD.temperatureSetting << " K" << std::endl << std::endl;

    stream << " Accel: " << std::endl;
    for (unsigned i = 1; i <= cD.accelCalib.size(); i++)
    {
        for (unsigned j = 1; j <= cD.accelCalib.size(); j++)
        {
            stream << cD.accelCalib(i, j) << "	";
        }
        stream << std::endl;
    }

    stream << std::endl << " Magnet S: " << std::endl;
    for (unsigned i = 1; i <= cD.magnetSoft.size(); i++)
    {
        for (unsigned j = 1; j <= cD.magnetSoft.size(); j++)
        {
            stream << cD.magnetSoft(i, j) << "	";
        }
        stream << std::endl;
    }

    stream << std::endl << " Maget H: " << std::endl;
    for (unsigned i = 1; i <= cD.magnetHard.size(); i++)
    {
        stream << cD.magnetHard(i) << std::endl;
    }
    stream << std::endl;
    return stream;
}

#endif //__SKYDIVE_USE_STL__
