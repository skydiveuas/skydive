package com.skydive.java.data;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class StateVector {

    public float lat, lon;
    public float absoluteAltitude;
    public float relativeAltitude;
    public float[] q = new float[3];
    public float[] vLoc = new float[3], vGlob = new float[3];
    public float[] aLocPure = new float[3], aGlobPure = new float[3];
    public float[] wLoc = new float[3];
    public int gpsFix;
    public float batteryVoltage;

    public StateVector() {
    }

    public StateVector(byte[] data) {
        this(data, 0);
    }

    public StateVector(byte[] data, int offset) {
        ByteBuffer buffer = ByteBuffer.wrap(data, offset, getSize());
        buffer.order(ByteOrder.LITTLE_ENDIAN);
    }

    public byte[] serialize() {
        byte[] out = new byte[getSize()];
        serialize(out);
        return out;
    }

    public void serialize(byte[] out) {
        if (out.length >= getSize()) {
            ByteBuffer buffer = ByteBuffer.wrap(out);
            buffer.order(ByteOrder.LITTLE_ENDIAN);

            System.arraycopy(buffer.array(), 0, out, 0, getSize());
        }
    }

    public static int getSize() {
        return 0;
    }
}
