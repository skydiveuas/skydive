// =========== roboLib ============
// ===  *** BARTOSZ NAWROT ***  ===
// ================================
#ifndef __CALIBRATION_SETTINGS__
#define __CALIBRATION_SETTINGS__

#ifdef __SKYDIVE_USE_STL__

#include <string>

#endif // __SKYDIVE_USE_STL__

#include "common/MathCore.hpp"

#include "ISignalPayloadMessage.hpp"

#include "SignalData.hpp"
#include "common/Flags.hpp"

/**
 * =============================================================================================
 * CalibrationSettings
 * =============================================================================================
 */
class CalibrationSettings : public ISignalPayloadMessage
{
public:
    enum BoardType
    {
        TYPE_UNKNOWN,
        TYPE_ULTIMATE_V5 = 5,
        TYPE_BASIC_V2 = 102,
        TYPE_BASIC_V3 = 103,
        TYPE_PRO_V1 = 201
    };

    enum FlagId
    {
        IS_GPS_CONNECTED,
        IS_EXTERNAL_MAGNETOMETER_USED,
        IS_BATTERY_MEASUREMENT_VALID
    };

    Vect3Df gyroOffset;
    Mat3Df accelCalib;
    Mat3Df magnetSoft;
    Vect3Df magnetHard;

    float altimeterSetting; // [hPa]
    float temperatureSetting; // [K]

    Mat4Df radioLevels;
    char pwmInputMapData[8];

    int boardType;

    Flags<unsigned> flags;

    CalibrationSettings(void);
    CalibrationSettings(const unsigned char* src);

    void serialize(unsigned char* dst) const;

    unsigned getDataSize(void) const;

    SignalData::Command getSignalDataType(void) const;
    SignalData::Command getSignalDataCommand(void) const;
    SignalData::Command getUploadAction(void) const;

    MessageType getMessageType(void) const;

    bool isValid(void) const;

    unsigned getCrc(void) const;

    void setCrc(void);

    ISignalPayloadMessage* clone(void) const;

    BoardType getBoardType() const;

    static CalibrationSettings createDefault(void);

    static Mat4Df getDefaultRadioLevels(void);

    void setDefaultPwmInputMap(void);
    void setPwmInputMap(char* map);

#ifdef __SKYDIVE_USE_STL__

    static std::string getBoardTypeString(BoardType boardType);

    std::string getBoardTypeString(void) const;

    static BoardType boardTypeFromString(const std::string& boardTypeString);

    friend std::ostream& operator << (std::ostream& stream, const CalibrationSettings& cD);

#endif //__SKYDIVE_USE_STL__

private:
    unsigned crcValue;
};

#endif // __CALIBRATION_SETTINGS__
