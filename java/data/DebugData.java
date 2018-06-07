package com.skydive.java.data;

import com.skydive.java.CommMessage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by Bartosz Nawrot on 2016-08-22.
 * Container tha stores most important telemetry data from drone:
 * - euler angles - rotation of drone
 * - position (lat, lon, alt), altitude is relative to start
 * - vLoc - speed of drone relatively to ground
 * - controller state - actual control command used by controller
 * - flags:
 * GPS fix | GPS 3D fix | low. bat. volt. | errorHandling | autopilotUsed | solver1 | solver2
 * - battery voltage [volts]
 */
public class DebugData {

    private float roll, pitch, yaw;

    private float latitude, longitude;
    private float relativeAltitude; // altitude relative to base location
    private float absoluteAltitude; // altitude above see level
    private float verticalVelocity;

    private float velocity; // velocity relative to XY earth plane [m/s]

    private float usedThrottle;
    private float distanceToBase; // [km]

    private ControllerState controllerState;
    private Flags flags;

    private byte battery;

    public DebugData() {
        this.flags = new Flags(8);
    }

    public DebugData(DebugData debugData) {
        this.roll = debugData.getRoll();
        this.pitch = debugData.getPitch();
        this.yaw = debugData.getYaw();
        this.latitude = debugData.getLatitude();
        this.longitude = debugData.getLongitude();
        this.relativeAltitude = debugData.getRelativeAltitude();
        this.absoluteAltitude = debugData.getAbsoluteAltitude();
        this.verticalVelocity = debugData.getVerticalVelocity();
        this.velocity = debugData.getVelocity();
        this.usedThrottle = debugData.getUsedThrottle();
        this.distanceToBase = debugData.getDistanceToBase();
        this.controllerState = debugData.getControllerState();
        this.flags = new Flags(8, debugData.getFlags());
        this.battery = debugData.getBattery();
    }

    public DebugData(final CommMessage message) {
        ByteBuffer buffer = ByteBuffer.wrap(message.getPayload());
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        this.roll = buffer.getFloat();
        this.pitch = buffer.getFloat();
        this.yaw = buffer.getFloat();
        this.latitude = buffer.getFloat();
        this.longitude = buffer.getFloat();
        this.relativeAltitude = buffer.getFloat();
        this.absoluteAltitude = buffer.getFloat();
        this.verticalVelocity = buffer.getFloat();
        this.velocity = buffer.getFloat();
        this.usedThrottle = buffer.getFloat();
        this.distanceToBase = buffer.getFloat();
        this.controllerState = ControllerState.getControllerState(buffer.getShort());
        this.flags = new Flags(8, buffer.get());
        this.battery = buffer.get();
    }

    public float getRoll() {
        return roll;
    }

    public void setRoll(float roll) {
        this.roll = roll;
    }

    public float getPitch() {
        return pitch;
    }

    public void setPitch(float pitch) {
        this.pitch = pitch;
    }

    public float getYaw() {
        return yaw;
    }

    public void setYaw(float yaw) {
        this.yaw = yaw;
    }

    public float getNormalYaw() {
        float yaw = this.yaw;
        if (yaw > 2 * Math.PI) {
            yaw -= (float) (2 * Math.PI);
        } else if (yaw < 0.0f) {
            yaw += (float) (2 * Math.PI);
        }
        return yaw;
    }

    public float getLatitude() {
        return latitude;
    }

    public void setLatitude(float latitude) {
        this.latitude = latitude;
    }

    public float getLongitude() {
        return longitude;
    }

    public void setLongitude(float longitude) {
        this.longitude = longitude;
    }

    public float getRelativeAltitude() {
        return relativeAltitude;
    }

    public void setRelativeAltitude(float relativeAltitude) {
        this.relativeAltitude = relativeAltitude;
    }

    public float getAbsoluteAltitude() {
        return absoluteAltitude;
    }

    public void setAbsoluteAltitude(float absoluteAltitude) {
        this.absoluteAltitude = absoluteAltitude;
    }

    public float getVerticalVelocity() {
        return verticalVelocity;
    }

    public void setVerticalVelocity(float verticalVelocity) {
        this.verticalVelocity = verticalVelocity;
    }

    public float getVelocity() {
        return velocity;
    }

    public void setVelocity(float velocity) {
        this.velocity = velocity;
    }

    public float getUsedThrottle() {
        return usedThrottle;
    }

    public void setUsedThrottle(float usedThrottle) {
        this.usedThrottle = usedThrottle;
    }

    public float getDistanceToBase() {
        return distanceToBase;
    }

    public void setDistanceToBase(float distanceToBase) {
        this.distanceToBase = distanceToBase;
    }

    public ControllerState getControllerState() {
        return controllerState;
    }

    public void setControllerState(ControllerState controllerState) {
        this.controllerState = controllerState;
    }

    public byte getFlags() {
        return (byte) flags.getFlags();
    }

    public void setFlags(byte flags) {
        this.flags = new Flags(8, flags);
    }

    public boolean getFlagState(FlagId id) {
        return flags.getFlagState(id.getValue());
    }

    public void setFLagState(FlagId id, boolean state) {
        flags.setFlagsState(id.getValue(), state);
    }

    public void setSolverMode(final ControlData.SolverMode solverMode) {
        // clean previous state
        flags.setFlags(flags.getFlags() & 0xFC);
        // set new solver flags
        flags.setFlags(flags.getFlags() | solverMode.getValue());
    }

    public ControlData.SolverMode getSolverMode() {
        return ControlData.SolverMode.getSolverMode((byte) (flags.getFlags() & 0x03));
    }

    public byte getBattery() {
        return battery;
    }

    public void setBattery(byte battery) {
        this.battery = battery;
    }

    public boolean isStopState() {
        return this.controllerState == ControllerState.STOP;
    }

    @Override
    public String toString() {
        return "DebugData:[ roll: " + String.valueOf(roll)
                + ", pitch: " + String.valueOf(pitch)
                + ", yaw: " + String.valueOf(yaw)
                + ", lat: " + String.valueOf(latitude)
                + ", lon: " + String.valueOf(longitude)
                + ", alt: " + String.valueOf(relativeAltitude)
                + ", Controller state: " + controllerState.toString() + "(" + String.valueOf(controllerState.getValue()) + ")"
                + " ]";
    }

    public CommMessage getMessage() {
        ByteBuffer buffer = ByteBuffer.allocate(CommMessage.getPayloadSizeByType(CommMessage.MessageType.CONTROL));
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        buffer.putFloat(getRoll());
        buffer.putFloat(getPitch());
        buffer.putFloat(getYaw());
        buffer.putFloat(getLatitude());
        buffer.putFloat(getLongitude());
        buffer.putFloat(getRelativeAltitude());
        buffer.putFloat(getAbsoluteAltitude());
        buffer.putFloat(getVerticalVelocity());
        buffer.putFloat(getVelocity());
        buffer.putFloat(getUsedThrottle());
        buffer.putFloat(getDistanceToBase());
        buffer.putShort(getControllerState().getValue());
        buffer.put((byte) flags.getFlags());
        buffer.put(battery);
        return new CommMessage(CommMessage.MessageType.CONTROL, buffer.array());
    }

    public enum ControllerState {
        IDLE((short) 0),
        // manual control
        MANUAL(ControlData.ControllerCommand.MANUAL.getValue()),
        // auto lading with specified descend rate
        AUTOLANDING(ControlData.ControllerCommand.AUTOLANDING.getValue()),
        // auto lading with specified descend rate and position hold
        AUTOLANDING_AP(ControlData.ControllerCommand.AUTOLANDING_AP.getValue()),
        // auto altitude hold, throttle value is now specifying descend/climb rate
        // th = 0 -> -v, th = 0.5 -> 0, th = 1.0 -> v
        HOLD_ALTITUDE(ControlData.ControllerCommand.HOLD_ALTITUDE.getValue()),
        // auto position hold, (hold altitude enabled)
        HOLD_POSITION(ControlData.ControllerCommand.HOLD_POSITION.getValue()),
        // autonomous back to base, climb 10 meters above start, cruise to base and auto land with AP
        BACK_TO_BASE(ControlData.ControllerCommand.BACK_TO_BASE.getValue()),
        // cruise via specific route and back to base
        VIA_ROUTE(ControlData.ControllerCommand.VIA_ROUTE.getValue()),
        // immediate STOP (even when fling)
        STOP(ControlData.ControllerCommand.STOP.getValue()),

        APPLICATION_LOOP((short) 3000),

        ERROR_CONNECTION(ControlData.ControllerCommand.ERROR_CONNECTION.getValue());

        private final short value;

        ControllerState(short value) {
            this.value = value;
        }

        public short getValue() {
            return value;
        }

        static public ControllerState getControllerState(short value) {
            for (ControllerState controllerState : ControllerState.values()) {
                if (controllerState.getValue() == value) {
                    return controllerState;
                }
            }
            return IDLE; // TODO throw some exception
        }

        @Override
        public String toString() {
            if (value == IDLE.getValue()) return "Idle";
            else if (value == MANUAL.getValue()) return "Manual";
            else if (value == AUTOLANDING.getValue()) return "Auto landing";
            else if (value == AUTOLANDING_AP.getValue()) return "Auto landing AP";
            else if (value == HOLD_ALTITUDE.getValue()) return "Hold altitude";
            else if (value == HOLD_POSITION.getValue()) return "Hold position";
            else if (value == BACK_TO_BASE.getValue()) return "Back to base";
            else if (value == VIA_ROUTE.getValue()) return "Via route";
            else if (value == STOP.getValue()) return "Stop";
            else if (value == APPLICATION_LOOP.getValue()) return "Application loop";
            else if (value == ERROR_CONNECTION.getValue()) return "Error connection";
            else return "Error type!";
        }
    }

    public enum FlagId {
        SOLVER_1(0),
        SOLVER_2(1),
        AUTOLANDING_ENABLED(2),
        AUTOPILOT_ENABLED(3),
        ERROR_HANDLING(4),
        LOW_BATTERY_VOLTAGE(5),
        GPS_FIX_3D(6),
        GPS_FIX(7);

        private final int value;

        FlagId(int value) {
            this.value = value;
        }

        int getValue() {
            return value;
        }
    }
}
