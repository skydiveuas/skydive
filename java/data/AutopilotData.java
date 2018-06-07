package com.skydive.java.data;

import com.skydive.java.CommMessage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by Bartosz Nawrot on 2016-08-22.
 * Autopilot data used to control drone in autopilot mode.
 * This data is sent to drone when target is changed and send back from drone as ACK.
 * Data:
 * - position (lat, lon, alt)
 * - flags:
 * ..............
 */
public class AutopilotData {

    private double latitude, longitude;
    private float absoluteAltitude;
    private float relativeAltitude;

    private Type type;

    private int flags;

    public AutopilotData() {
        type = Type.INVALID_TYPE;
    }

    public AutopilotData(CommMessage message) {
        ByteBuffer buffer = ByteBuffer.wrap(message.getPayload());
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        this.latitude = buffer.getDouble();
        this.longitude = buffer.getDouble();
        this.absoluteAltitude = buffer.getFloat();
        this.relativeAltitude = buffer.getFloat();
        this.type = Type.getType(buffer.getInt());
        this.flags = buffer.getInt();
    }

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }

    public float getAbsoluteAltitude() {
        return absoluteAltitude;
    }

    public void setAbsoluteAltitude(float absoluteAltitude) {
        this.absoluteAltitude = absoluteAltitude;
    }

    public float getRelativeAltitude() {
        return relativeAltitude;
    }

    public void setRelativeAltitude(float relativeAltitude) {
        this.relativeAltitude = relativeAltitude;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }

    public int getFlags() {
        return flags;
    }

    public void setFlags(int flags) {
        this.flags = flags;
    }

    @Override
    public String toString() {
        return "AutopilotData::" + type.toString() + "(" + String.valueOf(type.getValue()) + ")" +
                " lat: " + String.valueOf(latitude) + " lon: " + String.valueOf(longitude);
    }

    public CommMessage getMessage() {
        ByteBuffer buffer = ByteBuffer.allocate(CommMessage.getPayloadSizeByType(CommMessage.MessageType.AUTOPILOT));
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        buffer.putDouble(getLatitude());
        buffer.putDouble(getLongitude());
        buffer.putFloat(getAbsoluteAltitude());
        buffer.putFloat(getRelativeAltitude());
        buffer.putInt(getType().getValue());
        buffer.putInt(getFlags());
        return new CommMessage(CommMessage.MessageType.AUTOPILOT, buffer.array());
    }

    public boolean isEqual(AutopilotData autopilotData) {
        return this.latitude == autopilotData.latitude
                && this.longitude == autopilotData.longitude
                && this.absoluteAltitude == autopilotData.absoluteAltitude
                && this.relativeAltitude == autopilotData.relativeAltitude
                && this.type == autopilotData.type
                && this.flags == autopilotData.flags;
    }

    public enum Type {
        INVALID_TYPE(1000),
        BASE(2000), // from UAV
        BASE_ACK(2100),
        TARGET(3000), // from operator
        TARGET_ACK(3100),
        TARGET_NOT_ALLOWED_STATE(3200),
        TARGET_NOT_ALLOWED_SETTINGS(3300);

        private final int value;

        Type(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        static public Type getType(int value) {
            if (value == INVALID_TYPE.getValue()) return INVALID_TYPE;
            else if (value == BASE.getValue()) return BASE;
            else if (value == BASE_ACK.getValue()) return BASE_ACK;
            else if (value == TARGET.getValue()) return TARGET;
            else if (value == TARGET_ACK.getValue()) return TARGET_ACK;
            else if (value == TARGET_NOT_ALLOWED_STATE.getValue()) return TARGET_NOT_ALLOWED_STATE;
            else if (value == TARGET_NOT_ALLOWED_SETTINGS.getValue()) return TARGET_NOT_ALLOWED_SETTINGS;
            else return INVALID_TYPE; // TODO throw some exception
        }
    }

    public enum FlagId {
        ALTITUDE_DEFINED(0),
        AUTOLAND_AT_TARGET(1);

        private final int value;

        FlagId(int value) {
            this.value = value;
        }

        int getValue() {
            return value;
        }
    }
}
