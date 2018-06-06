package com.skydive.java;

import com.skydive.java.data.SignalData;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;

/**
 * Created by Bartosz Nawrot on 2016-10-13.
 */
public class CommMessage {
    //public static final int PROTOCOL_VERSION = 0xD6CE5A35;
    public static final int PROTOCOL_VERSION = 0xD6CE5A35 + 1;// 30-04-2018

    public static final int PREAMBLE_SIZE = 4;
    public static final int SIGNAL_COMMAND_SIZE = 4;
    public static final int CRC_SIZE = 2;

    public static final int SIGNAL_CONSTRAINT_SIZE = 8;
    public static final int SIGNAL_DATA_PAYLOAD_SIZE = 50;

    public static final int MAX_MESSAGE_SIZE =
            PREAMBLE_SIZE + SIGNAL_CONSTRAINT_SIZE + SIGNAL_DATA_PAYLOAD_SIZE + CRC_SIZE;

    public enum MessageType {
        EMPTY,
        SIGNAL,
        CONTROL,
        AUTOPILOT,
    }

    private MessageType type;
    private byte[] payload;
    private short crc;

    public CommMessage(MessageType type, byte[] payload) {
        this.type = type;
        this.payload = new byte[payload.length];
        System.arraycopy(payload, 0, this.payload, 0, payload.length);
        this.crc = computeCrc();
    }

    public MessageType getType() {
        return type;
    }

    public int getMessageSize() {
        return payload.length + PREAMBLE_SIZE + CRC_SIZE;
    }

    public byte[] getPayload() {
        return payload;
    }

    public byte[] getByteArray() {
        byte[] message = new byte[getMessageSize()];
        System.arraycopy(getPreambleByType(type), 0, message, 0, 4);
        System.arraycopy(payload, 0, message, 4, payload.length);
        message[getMessageSize() - 2] = (byte) (crc & 0xff);
        message[getMessageSize() - 1] = (byte) ((crc >> 8) & 0xff);
        return message;
    }

    public boolean isValid() {
        return crc == computeCrc();
    }

    private short computeCrc() {
        return computeCrc16(payload);
    }

    public static byte[] getPreambleByType(MessageType type) {
        switch (type) {
            case CONTROL:
                return new byte[]{'$', '$', '$', 0};
            case SIGNAL:
                return new byte[]{'%', '%', '%', 0};
            case AUTOPILOT:
                return new byte[]{'^', '^', '^', 0};
            default:
                // TODO throw some error
                return new byte[]{'a', 'a', 'a', 'a'};
        }
    }

    public static MessageType getPreambleTypeByChar(byte b) {
        switch (b) {
            case '$':
                return MessageType.CONTROL;
            case '%':
                return MessageType.SIGNAL;
            case '^':
                return MessageType.AUTOPILOT;
            default:
                // TODO throw some error
                return MessageType.EMPTY;
        }
    }

    public static int getPayloadSizeByType(MessageType type) {
        switch (type) {
            case CONTROL:
                return 58;
            case SIGNAL:
                return 8;
            case AUTOPILOT:
                return 32;
            default:
                // TODO throw some error
                return -1;
        }
    }

    public static ArrayList<CommMessage> buildMessagesList(final SignalData.Command dataCommand, final byte[] dataArray) {
        ArrayList<CommMessage> result = new ArrayList<>();

        final short messagesCount = (short) ((double) dataArray.length / CommMessage.SIGNAL_DATA_PAYLOAD_SIZE + 0.99999);

        ByteBuffer buffer = ByteBuffer.allocate(4);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        byte[] payload = new byte[CommMessage.SIGNAL_CONSTRAINT_SIZE + CommMessage.SIGNAL_DATA_PAYLOAD_SIZE];

        // set CALIBRATION_SETTINGS_DATA command
        buffer.putInt(dataCommand.getValue());
        System.arraycopy(buffer.array(), 0, payload, 0, 4);

        // set max packets to message
        buffer.clear();
        buffer.putShort(messagesCount);
        System.arraycopy(buffer.array(), 0, payload, 4, 2);

        for (short i = 0; i < messagesCount; i++) {
            // set actual packet number to message
            buffer.clear();
            buffer.putShort(i);
            System.arraycopy(buffer.array(), 0, payload, 6, 2);

            // copy part of array to message
            if (i == messagesCount - 1) {
                final int sent = (messagesCount - 1) * CommMessage.SIGNAL_DATA_PAYLOAD_SIZE;
                final int left = dataArray.length - sent;
                System.arraycopy(
                        dataArray, i * CommMessage.SIGNAL_DATA_PAYLOAD_SIZE,
                        payload, 8,
                        left);
            } else {
                System.arraycopy(
                        dataArray, i * CommMessage.SIGNAL_DATA_PAYLOAD_SIZE,
                        payload, 8,
                        CommMessage.SIGNAL_DATA_PAYLOAD_SIZE);
            }

            CommMessage message = new CommMessage(CommMessage.MessageType.SIGNAL, payload);
            result.add(message);
        }
        return result;
    }

    static public short computeCrc16(byte[] data) {
        return computeCrc16(data, data.length);
    }

    static public short computeCrc16(byte[] data, int dataSize) {
        int crcShort = 0;
        for (int i = 0; i < dataSize; ++i) {
            byte b = data[i];
            crcShort = ((crcShort >>> 8) | (crcShort << 8)) & 0xffff;
            crcShort ^= (b & 0xff);
            crcShort ^= ((crcShort & 0xff) >> 4);
            crcShort ^= (crcShort << 12) & 0xffff;
            crcShort ^= ((crcShort & 0xFF) << 5) & 0xffff;
        }
        crcShort &= 0xffff;
        return (short) crcShort;
    }

    static public int computeCrc32(byte[] data) {
        return computeCrc32(data, data.length);
    }

    static public int computeCrc32(byte[] data, int dataSize) {
        int crc = 0;
        crc = ~crc;
        for (int i = 0; i < dataSize; ++i)
        {
            byte b = data[i];
            int bb = (b >= 0 ? b : (256 + b)); // turn 'byte' to 'unsigned char' value
            crc ^= bb;
            for (int k = 0; k < 8; k++) {
                crc = ((crc & 1) != 0) ? (crc >> 1) ^ 0x82f63b78 : crc >> 1;
            }
        }
        return ~crc;
    }

    public static String byteToHexString(byte b) {
        String ret = "";
        int intVal = b & 0xff;
        if (intVal < 0x10) ret += "0";
        ret += Integer.toHexString(intVal);
        return ret;
    }

    public static String byteArrayToHexString(byte[] in) {
        String ret = "";
        for (byte b : in) {
            ret += byteToHexString(b);
        }
        return ret;
    }

    @Override
    public String toString() {
        byte[] crcArray = new byte[2];
        crcArray[1] = (byte) (crc & 0xff);
        crcArray[0] = (byte) ((crc >> 8) & 0xff);
        return byteArrayToHexString(getPreambleByType(type))
                + " | " + byteArrayToHexString(payload)
                + " | " + byteArrayToHexString(crcArray);
    }
}
