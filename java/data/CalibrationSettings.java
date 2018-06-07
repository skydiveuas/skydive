package com.skydive.java.data;

import com.skydive.java.CommMessage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by Bartosz Nawrot on 2016-10-14.
 */
public class CalibrationSettings implements SignalPayloadData {

    private float[] gyroOffset; // Vect3Df
    private float[] accelCalib; // Mat3Df
    private float[] magnetSoft; // Mat3Df
    private float[] magnetHard; // Vect3Df
    private float altimeterSetting;
    private float temperatureSetting;
    private float[] radioLevels; // Mat4Df
    private byte[] pwmInputLevels; // 8 params
    private int boardTypeValue;
    private Flags flags;
    private int crcValue;

    public CalibrationSettings() {
        gyroOffset = new float[3];
        accelCalib = new float[9];
        magnetSoft = new float[9];
        magnetHard = new float[3];
        radioLevels = new float[16];
        pwmInputLevels = new byte[8];
        flags = new Flags(32, 0);

        accelCalib[0] = 1.0f;
        accelCalib[4] = 1.0f;
        accelCalib[8] = 1.0f;

        magnetSoft[0] = 1.0f;
        magnetSoft[4] = 1.0f;
        magnetSoft[8] = 1.0f;

        altimeterSetting = 1013.2f;
        temperatureSetting = 288.15f;

        boardTypeValue = BoardType.TYPE_UNKNOWN.getValue();

        crcValue = computeCrc();
    }

    public CalibrationSettings(final byte[] dataArray) {
        gyroOffset = new float[3];
        accelCalib = new float[9];
        magnetSoft = new float[9];
        magnetHard = new float[3];
        radioLevels = new float[16];
        pwmInputLevels = new byte[8];
        flags = new Flags(32, 0);

        ByteBuffer buffer = ByteBuffer.wrap(dataArray);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        for (int i = 0; i < gyroOffset.length; i++) {
            gyroOffset[i] = buffer.getFloat();
        }
        for (int i = 0; i < accelCalib.length; i++) {
            accelCalib[i] = buffer.getFloat();
        }
        for (int i = 0; i < magnetSoft.length; i++) {
            magnetSoft[i] = buffer.getFloat();
        }
        for (int i = 0; i < magnetHard.length; i++) {
            magnetHard[i] = buffer.getFloat();
        }

        altimeterSetting = buffer.getFloat();
        temperatureSetting = buffer.getFloat();

        for (int i = 0; i < radioLevels.length; i++) {
            radioLevels[i] = buffer.getFloat();
        }
        for (int i = 0; i < pwmInputLevels.length; i++) {
            pwmInputLevels[i] = buffer.get();
        }

        boardTypeValue = buffer.getInt();
        flags.setFlags(buffer.getInt());
        crcValue = buffer.getInt();
    }

    @Override
    public SignalData.Command getDataCommand() {
        return SignalData.Command.CALIBRATION_SETTINGS;
    }

    @Override
    public SignalData.Command getDataType() {
        return SignalData.Command.CALIBRATION_SETTINGS_DATA;
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
        return 188;
    }

    public byte[] serialize() {
        byte[] dataArray = new byte[getDataArraySize()];
        ByteBuffer buffer = ByteBuffer.wrap(dataArray);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        for (float aGyroOffset : gyroOffset) {
            buffer.putFloat(aGyroOffset);
        }
        for (float anAccelCalib : accelCalib) {
            buffer.putFloat(anAccelCalib);
        }
        for (float aMagnetSoft : magnetSoft) {
            buffer.putFloat(aMagnetSoft);
        }
        for (float aMagnetHard : magnetHard) {
            buffer.putFloat(aMagnetHard);
        }

        buffer.putFloat(altimeterSetting);
        buffer.putFloat(temperatureSetting);

        for (float radioLevel : radioLevels) {
            buffer.putFloat(radioLevel);
        }
        for (byte pwmInputLevel : pwmInputLevels) {
            buffer.put(pwmInputLevel);
        }

        buffer.putInt(boardTypeValue);
        buffer.putInt(flags.getFlags());
        buffer.putInt(crcValue);

        return dataArray;
    }

    public void setGyroOffset(float[] gyroOffset) {
        this.gyroOffset = gyroOffset;
    }

    public void setAccelCalib(float[] accelCalib) {
        this.accelCalib = accelCalib;
    }

    public void setMagnetSoft(float[] magnetSoft) {
        this.magnetSoft = magnetSoft;
    }

    public void setMagnetHard(float[] magnetHard) {
        this.magnetHard = magnetHard;
    }

    public void setAltimeterSetting(float altimeterSetting) {
        this.altimeterSetting = altimeterSetting;
    }

    public void setTemperatureSetting(float temperatureSetting) {
        this.temperatureSetting = temperatureSetting;
    }

    public void setRadioLevels(float[] radioLevels) {
        this.radioLevels = radioLevels;
    }

    public void setPwmInputLevels(byte[] pwmInputLevels) {
        this.pwmInputLevels = pwmInputLevels;
    }

    public void setBoardTypeValue(int boardTypeValue) {
        this.boardTypeValue = boardTypeValue;
    }

    public void setFlags(Flags flags) {
        this.flags = flags;
    }

    public void setFlagState(FlagId id, boolean state) {
        flags.setFlagsState(id.getValue(), state);
    }

    public void setCrc() {
        this.crcValue = computeCrc();
    }

    public float[] getGyroOffset() {
        return gyroOffset;
    }

    public float[] getAccelCalib() {
        return accelCalib;
    }

    public float[] getMagnetSoft() {
        return magnetSoft;
    }

    public float[] getMagnetHard() {
        return magnetHard;
    }

    public float getAltimeterSetting() {
        return altimeterSetting;
    }

    public float getTemperatureSetting() {
        return temperatureSetting;
    }

    public float[] getRadioLevels() {
        return radioLevels;
    }

    public byte[] getPwmInputLevels() {
        return pwmInputLevels;
    }

    public BoardType getBoardType() {
        return BoardType.getBoardType(boardTypeValue);
    }

    public boolean getFlagState(FlagId id) {
        return flags.getFlagState(id.getValue());
    }

    public enum BoardType {
        TYPE_UNKNOWN(0),
        TYPE_ULTIMATE_V5(5),
        TYPE_BASIC_V2(102),
        TYPE_BASIC_V3(103),
        TYPE_PRO_V1(201);

        private final int value;

        BoardType(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public static BoardType getBoardType(final int value) {
            for (BoardType boardType : BoardType.values()) {
                if (boardType.getValue() == value) {
                    return boardType;
                }
            }
            return TYPE_UNKNOWN; // TODO throw some exception
        }
    }

    public enum FlagId {
        IS_GPS_CONNECTED(0),
        IS_EXTERNAL_MAGNETOMETER_USED(1),
        IS_BATTERY_MEASUREMENT_VALID(2);

        private final int value;

        FlagId(int value) {
            this.value = value;
        }

        int getValue() {
            return value;
        }
    }
}