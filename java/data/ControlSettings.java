package com.skydive.java.data;

import com.skydive.java.CommMessage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;

import static com.skydive.java.data.ControlSettings.StickMovementMode.COPTER;

/**
 * Created by Bartosz Nawrot on 2016-10-14.
 */
public class ControlSettings implements SignalPayloadData {

    // base control settings
    private int uavType;
    private int initialSolverMode;
    private int manualThrottleMode;

    // auto landing settings
    private float autoLandingDescendRate;
    private float maxAutoLandingTime;

    // control values
    private float maxRollPitchControlValue;
    private float maxYawControlValue;

    // regulator control settings
    // stabilization
    private float[] pidRollRate, pidPitchRate, pidYawRate; // Vect3Df
    // angle
    private float rollProp, pitchProp, yawProp;

    // throttle controller settings
    private float maxVerticalAutoVelocity;
    private float altPositionProp;
    private float altVelocityProp;
    private float[] pidThrottleAccel; // Vect3Df

    // throttle controller stick settings
    private float throttleAltRateProp;

    // autopilot settings
    private float maxAutoAngle;
    private float maxAutoVelocity;
    private float autoPositionProp;
    private float autoVelocityProp;
    private float[] pidAutoAccel; // Vect3Df

    // autopilot stick settings
    private float stickPositionRateProp;
    private int stickMovementMode;

    // battery type
    private int batteryType;

    // error handling type
    private int errorHandlingAction;

    // esc controller PWM frequency
    private int escPwmFreq;

    // gps sensors position in rigid body coordinate system
    private float[] gpsSensorPosition; // Vect3Df

    // flags for any boolean settings
    private Flags flags;

    private int crcValue;

    public ControlSettings() {
        pidRollRate = new float[3];
        pidPitchRate = new float[3];
        pidYawRate = new float[3];
        pidThrottleAccel = new float[3];
        pidAutoAccel = new float[3];
        gpsSensorPosition = new float[3];
        flags = new Flags(32, 0);

        uavType = UavType.QUADROCOPTER_X.getValue();
        initialSolverMode = ControlData.SolverMode.ANGLE_NO_YAW.getValue();
        manualThrottleMode = ThrottleMode.DYNAMIC.getValue();

        rollProp = 0.0f;
        pitchProp = 0.0f;
        yawProp = 0.0f;
        autoLandingDescendRate = 0.0f;
        maxAutoLandingTime = 0.0f;
        maxRollPitchControlValue = 0.0f;
        maxYawControlValue = 0.0f;

        // throttle controller
        maxVerticalAutoVelocity = 0.0f;
        altPositionProp = 0.0f;
        altVelocityProp = 0.0f;
        throttleAltRateProp = 0.0f;

        // autopilot
        autoPositionProp = 0.0f;
        autoVelocityProp = 0.0f;
        maxAutoAngle = 0.0f;
        maxAutoVelocity = 0.0f;
        stickPositionRateProp = 0.0f;
        stickMovementMode = COPTER.getValue();

        batteryType = BatteryType.UNDEFINED.getValue();
        errorHandlingAction = DebugData.ControllerState.AUTOLANDING.getValue();
        escPwmFreq = EscPwmFreq.MEDIUM.getValue();

        crcValue = computeCrc();
    }

    public ControlSettings(final byte[] dataArray) {
        pidRollRate = new float[3];
        pidPitchRate = new float[3];
        pidYawRate = new float[3];
        pidThrottleAccel = new float[3];
        pidAutoAccel = new float[3];
        gpsSensorPosition = new float[3];
        flags = new Flags(32, 0);

        ByteBuffer buffer = ByteBuffer.wrap(dataArray);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        uavType = buffer.getInt();
        initialSolverMode = buffer.getInt();
        manualThrottleMode = buffer.getInt();
        autoLandingDescendRate = buffer.getFloat();
        maxAutoLandingTime = buffer.getFloat();
        maxRollPitchControlValue = buffer.getFloat();
        maxYawControlValue = buffer.getFloat();
        for (int i = 0; i < pidRollRate.length; i++) {
            pidRollRate[i] = buffer.getFloat();
        }
        for (int i = 0; i < pidPitchRate.length; i++) {
            pidPitchRate[i] = buffer.getFloat();
        }
        for (int i = 0; i < pidYawRate.length; i++) {
            pidYawRate[i] = buffer.getFloat();
        }
        rollProp = buffer.getFloat();
        pitchProp = buffer.getFloat();
        yawProp = buffer.getFloat();
        maxVerticalAutoVelocity = buffer.getFloat();
        altPositionProp = buffer.getFloat();
        altVelocityProp = buffer.getFloat();
        for (int i = 0; i < pidThrottleAccel.length; i++) {
            pidThrottleAccel[i] = buffer.getFloat();
        }
        throttleAltRateProp = buffer.getFloat();
        maxAutoAngle = buffer.getFloat();
        maxAutoVelocity = buffer.getFloat();
        autoPositionProp = buffer.getFloat();
        autoVelocityProp = buffer.getFloat();
        for (int i = 0; i < pidAutoAccel.length; i++) {
            pidAutoAccel[i] = buffer.getFloat();
        }
        stickPositionRateProp = buffer.getFloat();
        stickMovementMode = buffer.getInt();
        batteryType = buffer.getInt();
        errorHandlingAction = buffer.getInt();
        escPwmFreq = buffer.getInt();
        for (int i = 0; i < gpsSensorPosition.length; i++) {
            gpsSensorPosition[i] = buffer.getFloat();
        }
        flags.setFlags(buffer.getInt());
        crcValue = buffer.getInt();
    }

    @Override
    public SignalData.Command getDataCommand() {
        return SignalData.Command.CONTROL_SETTINGS;
    }

    @Override
    public SignalData.Command getDataType() {
        return SignalData.Command.CONTROL_SETTINGS_DATA;
    }

    private int computeCrc() {
        // compute CRC value from whole data, excluding last 4 bytes (CRC value)
        return CommMessage.computeCrc32(Arrays.copyOfRange(serialize(), 0, getDataArraySize() - 4));
    }

    public boolean isValid() {
        return crcValue == computeCrc();
    }

    @Override
    public ArrayList<CommMessage> getMessages() {
        return CommMessage.buildMessagesList(getDataType(), serialize());
    }

    private int getDataArraySize() {
        return 172;
    }

    public byte[] serialize() {
        byte[] dataArray = new byte[getDataArraySize()];
        ByteBuffer buffer = ByteBuffer.wrap(dataArray);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        buffer.putInt(uavType);
        buffer.putInt(initialSolverMode);
        buffer.putInt(manualThrottleMode);
        buffer.putFloat(autoLandingDescendRate);
        buffer.putFloat(maxAutoLandingTime);
        buffer.putFloat(maxRollPitchControlValue);
        buffer.putFloat(maxYawControlValue);
        for (float v : pidRollRate) {
            buffer.putFloat(v);
        }
        for (float v : pidPitchRate) {
            buffer.putFloat(v);
        }
        for (float v : pidYawRate) {
            buffer.putFloat(v);
        }
        buffer.putFloat(rollProp);
        buffer.putFloat(pitchProp);
        buffer.putFloat(yawProp);
        buffer.putFloat(maxVerticalAutoVelocity);
        buffer.putFloat(altPositionProp);
        buffer.putFloat(altVelocityProp);
        for (float v : pidThrottleAccel) {
            buffer.putFloat(v);
        }
        buffer.putFloat(throttleAltRateProp);
        buffer.putFloat(maxAutoAngle);
        buffer.putFloat(maxAutoVelocity);
        buffer.putFloat(autoPositionProp);
        buffer.putFloat(autoVelocityProp);
        for (float v : pidAutoAccel) {
            buffer.putFloat(v);
        }
        buffer.putFloat(stickPositionRateProp);
        buffer.putInt(stickMovementMode);
        buffer.putInt(batteryType);
        buffer.putInt(errorHandlingAction);
        buffer.putInt(escPwmFreq);
        for (float v : gpsSensorPosition) {
            buffer.putFloat(v);
        }
        buffer.putInt(flags.getFlags());
        buffer.putInt(crcValue);

        return dataArray;
    }

    public enum UavType {
        TRICOPTER_REAR(1000),
        TRICOPTER_FRONT(1500),
        QUADROCOPTER_X(2000),
        QUADROCOPTER_PLUS(2500),
        HEXACOPTER_X(3000),
        HEXACOPTER_PLUS(3500),
        OCTOCOPTER_X(4000),
        OCTOCOPTER_PLUS(4500);

        private final int value;

        UavType(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static UavType getUavType(final int value) {
            for (UavType uavType : UavType.values()) {
                if (uavType.getValue() == value) {
                    return uavType;
                }
            }
            return QUADROCOPTER_X; // TODO throw some exception
        }
    }

    public enum ThrottleMode {
        STATIC(10),
        DYNAMIC(20),
        RATE(30),
        ALTITUDE(40);

        private final int value;

        ThrottleMode(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static ThrottleMode getThrottleMode(final int value) {
            for (ThrottleMode throttleMode : ThrottleMode.values()) {
                if (throttleMode.getValue() == value) {
                    return throttleMode;
                }
            }
            return DYNAMIC; // TODO throw some exception
        }
    }

    public enum StickMovementMode {
        COPTER(0),
        GEOGRAPHIC(1),
        BASE_POINT(2);

        private final int value;

        StickMovementMode(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static StickMovementMode getStickMovementMode(final int value) {
            for (StickMovementMode stickMovementMode : StickMovementMode.values()) {
                if (stickMovementMode.getValue() == value) {
                    return stickMovementMode;
                }
            }
            return COPTER; // TODO throw some exception
        }
    }

    public enum BatteryType {
        UNDEFINED(0),
        BATTERY_2S(2),
        BATTERY_3S(3),
        BATTERY_4S(4),
        BATTERY_5S(5),
        BATTERY_6S(6);

        private final int value;

        BatteryType(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static BatteryType getBatteryType(final int value) {
            for (BatteryType batteryType : BatteryType.values()) {
                if (batteryType.getValue() == value) {
                    return batteryType;
                }
            }
            return UNDEFINED; // TODO throw some exception
        }
    }

    public enum EscPwmFreq {
        SLOW(100),
        MEDIUM(200),
        FAST(300),
        VERY_FAST(400),
        ONESHOT_125(3200);

        private final int value;

        EscPwmFreq(int value) {
            this.value = value;
        }

        int getValue() {
            return value;
        }
    }

    public enum FlagId {
        ENABLE_FLIGHT_LOGGER(0),
        ALLOW_DYNAMIC_AUTOPILOT(1),
        GPS_SENSORS_POSITION_DEFINED(2);

        private final int value;

        FlagId(int value) {
            this.value = value;
        }

        int getValue() {
            return value;
        }
    }

    public int getUavType() {
        // TODO fix it, should return ENUM
        return uavType;
    }

    public int getInitialSolverMode() {
        // TODO fix it, should return ENUM
        return initialSolverMode;
    }

    public int getManualThrottleMode() {
        // TODO fix it, should return ENUM
        return manualThrottleMode;
    }

    public float getAutoLandingDescendRate() {
        return autoLandingDescendRate;
    }

    public float getMaxAutoLandingTime() {
        return maxAutoLandingTime;
    }

    public float getMaxRollPitchControlValue() {
        return maxRollPitchControlValue;
    }

    public float getMaxYawControlValue() {
        return maxYawControlValue;
    }

    public float[] getPidRollRate() {
        return pidRollRate;
    }

    public float[] getPidPitchRate() {
        return pidPitchRate;
    }

    public float[] getPidYawRate() {
        return pidYawRate;
    }

    public float getRollProp() {
        return rollProp;
    }

    public float getPitchProp() {
        return pitchProp;
    }

    public float getYawProp() {
        return yawProp;
    }

    public float getMaxVerticalAutoVelocity() {
        return maxVerticalAutoVelocity;
    }

    public float getAltPositionProp() {
        return altPositionProp;
    }

    public float getAltVelocityProp() {
        return altVelocityProp;
    }

    public float[] getPidThrottleAccel() {
        return pidThrottleAccel;
    }

    public float getThrottleAltRateProp() {
        return throttleAltRateProp;
    }

    public float getMaxAutoAngle() {
        return maxAutoAngle;
    }

    public float getMaxAutoVelocity() {
        return maxAutoVelocity;
    }

    public float getAutoPositionProp() {
        return autoPositionProp;
    }

    public float getAutoVelocityProp() {
        return autoVelocityProp;
    }

    public float[] getPidAutoAccel() {
        return pidAutoAccel;
    }

    public float getStickPositionRateProp() {
        return stickPositionRateProp;
    }

    public int getStickMovementMode() {
        // TODO fix it, should return ENUM
        return stickMovementMode;
    }

    public int getBatteryType() {
        // TODO fix it, should return ENUM
        return batteryType;
    }

    public int getErrorHandlingAction() {
        // TODO fix it, should return ENUM
        return errorHandlingAction;
    }

    public int getEscPwmFreq() {
        // TODO fix it, should return ENUM
        return escPwmFreq;
    }

    public float[] getGpsSensorPosition() {
        return gpsSensorPosition;
    }

    public Flags getFlags() {
        return flags;
    }

    public void setUavType(int uavType) {
        // TODO fix it, should return ENUM
        this.uavType = uavType;
    }

    public void setInitialSolverMode(int initialSolverMode) {
        // TODO fix it, should return ENUM
        this.initialSolverMode = initialSolverMode;
    }

    public void setManualThrottleMode(int manualThrottleMode) {
        // TODO fix it, should return ENUM
        this.manualThrottleMode = manualThrottleMode;
    }

    public void setAutoLandingDescendRate(float autoLandingDescendRate) {
        this.autoLandingDescendRate = autoLandingDescendRate;
    }

    public void setMaxAutoLandingTime(float maxAutoLandingTime) {
        this.maxAutoLandingTime = maxAutoLandingTime;
    }

    public void setMaxRollPitchControlValue(float maxRollPitchControlValue) {
        this.maxRollPitchControlValue = maxRollPitchControlValue;
    }

    public void setMaxYawControlValue(float maxYawControlValue) {
        this.maxYawControlValue = maxYawControlValue;
    }

    public void setPidRollRate(float[] pidRollRate) {
        this.pidRollRate = pidRollRate;
    }

    public void setPidPitchRate(float[] pidPitchRate) {
        this.pidPitchRate = pidPitchRate;
    }

    public void setPidYawRate(float[] pidYawRate) {
        this.pidYawRate = pidYawRate;
    }

    public void setRollProp(float rollProp) {
        this.rollProp = rollProp;
    }

    public void setPitchProp(float pitchProp) {
        this.pitchProp = pitchProp;
    }

    public void setYawProp(float yawProp) {
        this.yawProp = yawProp;
    }

    public void setMaxVerticalAutoVelocity(float maxVerticalAutoVelocity) {
        this.maxVerticalAutoVelocity = maxVerticalAutoVelocity;
    }

    public void setAltPositionProp(float altPositionProp) {
        this.altPositionProp = altPositionProp;
    }

    public void setAltVelocityProp(float altVelocityProp) {
        this.altVelocityProp = altVelocityProp;
    }

    public void setPidThrottleAccel(float[] pidThrottleAccel) {
        this.pidThrottleAccel = pidThrottleAccel;
    }

    public void setThrottleAltRateProp(float throttleAltRateProp) {
        this.throttleAltRateProp = throttleAltRateProp;
    }

    public void setMaxAutoAngle(float maxAutoAngle) {
        this.maxAutoAngle = maxAutoAngle;
    }

    public void setMaxAutoVelocity(float maxAutoVelocity) {
        this.maxAutoVelocity = maxAutoVelocity;
    }

    public void setAutoPositionProp(float autoPositionProp) {
        this.autoPositionProp = autoPositionProp;
    }

    public void setAutoVelocityProp(float autoVelocityProp) {
        this.autoVelocityProp = autoVelocityProp;
    }

    public void setPidAutoAccel(float[] pidAutoAccel) {
        this.pidAutoAccel = pidAutoAccel;
    }

    public void setStickPositionRateProp(float stickPositionRateProp) {
        this.stickPositionRateProp = stickPositionRateProp;
    }

    public void setStickMovementMode(int stickMovementMode) {
        // TODO fix it, should return ENUM
        this.stickMovementMode = stickMovementMode;
    }

    public void setBatteryType(int batteryType) {
        // TODO fix it, should return ENUM
        this.batteryType = batteryType;
    }

    public void setErrorHandlingAction(int errorHandlingAction) {
        // TODO fix it, should return ENUM
        this.errorHandlingAction = errorHandlingAction;
    }

    public void setEscPwmFreq(int escPwmFreq) {
        // TODO fix it, should return ENUM
        this.escPwmFreq = escPwmFreq;
    }

    public void setGpsSensorPosition(float[] gpsSensorPosition) {
        this.gpsSensorPosition = gpsSensorPosition;
    }

    public void setCrc() {
        this.crcValue = computeCrc();
    }
}
