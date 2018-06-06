package com.skydive.java.data;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class ImuData {

    public float[] omega = new float[3];
    public float[] accel = new float[3];
    public float[] magnet = new float[3];
    public float pressure;

    public ImuData() {
    }

    public ImuData(byte[] data) {
        this(data, 0);
    }

    public ImuData(byte[] data, int offset) {
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
