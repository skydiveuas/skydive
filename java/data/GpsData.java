package com.skydive.java.data;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class GpsData {

    public double lat, lon;
    public float alt;
    public float speed, course;
    public float HDOP;
    public boolean fix, justFixed, justFixLost;

    public GpsData() {
    }

    public GpsData(byte[] data) {
        this(data, 0);
    }

    public GpsData(byte[] data, int offset) {
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
