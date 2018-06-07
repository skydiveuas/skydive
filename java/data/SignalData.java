package com.skydive.java.data;

import com.skydive.java.CommMessage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by Bartosz Nawrot on 2016-10-13.
 */
public class SignalData {
    private int commandValue;
    private int parameterValue;

    public SignalData(Command command, Parameter parameter) {
        this.commandValue = command.getValue();
        this.parameterValue = parameter.getValue();
    }

    public SignalData(Command command, int parameterValue) {
        this.commandValue = command.getValue();
        this.parameterValue = parameterValue;
    }

    public SignalData(CommMessage message) {
        ByteBuffer buffer = ByteBuffer.wrap(message.getPayload());
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        this.commandValue = buffer.getInt();
        this.parameterValue = buffer.getInt();
    }

    public Command getCommand() {
        return Command.getCommand(commandValue);
    }

    public Parameter getParameter() {
        return Parameter.getParameter(parameterValue);
    }

    public int getParameterValue() {
        return parameterValue;
    }

    public boolean equals(SignalData command) {
        return this.getCommand() == command.getCommand() && this.getParameter() == command.getParameter();
    }

    public CommMessage getMessage() {
        ByteBuffer buffer = ByteBuffer.allocate(CommMessage.getPayloadSizeByType(CommMessage.MessageType.SIGNAL));
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        buffer.putInt(commandValue);
        buffer.putInt(parameterValue);
        return new CommMessage(CommMessage.MessageType.SIGNAL, buffer.array());
    }

    public static SignalData.Command parseCommand(final byte[] src) {
        ByteBuffer buffer = ByteBuffer.wrap(src, 0, 4);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        return SignalData.Command.getCommand(buffer.getInt());
    }

    public static short parseAllPacketsNumber(final byte[] src) {
        ByteBuffer buffer = ByteBuffer.wrap(src, CommMessage.SIGNAL_COMMAND_SIZE, 2);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        return buffer.getShort();
    }

    public static short parseActualPacketNumber(final byte[] src) {
        ByteBuffer buffer = ByteBuffer.wrap(src, CommMessage.SIGNAL_COMMAND_SIZE + 2, 2);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        return buffer.getShort();
    }

    public static boolean hasPayload(final SignalData.Command command) {
        switch (command) {
            case CALIBRATION_SETTINGS_DATA:
            case CONTROL_SETTINGS_DATA:
            case ROUTE_CONTAINER_DATA:
                return true;

            default:
                return false;
        }
    }

    @Override
    public String toString() {
        return "SignalData( " + getCommand().toString() + ", " + getParameter().toString() + " )";
    }

    public enum Command {
        DUMMY(0),

        START_CMD(100007),

        APP_LOOP(100008),
        FLIGHT_LOOP(100009),
        CALIBRATE_ACCEL(100010),
        CALIBRATE_MAGNET(100011),
        CALIBRATE_ESC(100012),
        UPLOAD_SETTINGS(100013),
        DOWNLOAD_SETTINGS(100014),
        CALIBRATE_RADIO(100015),
        CHECK_RADIO(100016),
        SOFTWARE_UPGRADE(100017),
        SYSTEM_RESET(100018),
        UPLOAD_ROUTE(100019),
        DOWNLOAD_ROUTE(100020),
        CONFIGURE_WIFI(100021),
        SENSORS_LOGGER(100022),

        PING_VALUE(100023),

        CALIBRATION_SETTINGS(100024),
        CALIBRATION_SETTINGS_DATA(100025),
        CONTROL_SETTINGS(100026),
        CONTROL_SETTINGS_DATA(100027),
        ROUTE_CONTAINER(100028),
        ROUTE_CONTAINER_DATA(100029),
        WIFI_CONFIGURATION(100030),
        WIFI_CONFIGURATION_DATA(100031),

        // TODO sort these commands at new release
        WHO_AM_I_VALUE(100032), // board type (CalibrationSettings::BoardType)
        PROTOCOL_VERSION_VALUE(100033),
        PROTOCOL_VERSION(100034);

        private final int value;

        Command(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        static public Command getCommand(int value) {
            if (value == DUMMY.getValue()) return DUMMY;
            else if (value == START_CMD.getValue()) return START_CMD;
            else if (value == APP_LOOP.getValue()) return APP_LOOP;
            else if (value == FLIGHT_LOOP.getValue()) return FLIGHT_LOOP;
            else if (value == CALIBRATE_ACCEL.getValue()) return CALIBRATE_ACCEL;
            else if (value == CALIBRATE_MAGNET.getValue()) return CALIBRATE_MAGNET;
            else if (value == CALIBRATE_ESC.getValue()) return CALIBRATE_ESC;
            else if (value == UPLOAD_SETTINGS.getValue()) return UPLOAD_SETTINGS;
            else if (value == DOWNLOAD_SETTINGS.getValue()) return DOWNLOAD_SETTINGS;
            else if (value == CALIBRATE_RADIO.getValue()) return CALIBRATE_RADIO;
            else if (value == CHECK_RADIO.getValue()) return CHECK_RADIO;
            else if (value == SOFTWARE_UPGRADE.getValue()) return SOFTWARE_UPGRADE;
            else if (value == SYSTEM_RESET.getValue()) return SYSTEM_RESET;
            else if (value == UPLOAD_ROUTE.getValue()) return UPLOAD_ROUTE;
            else if (value == DOWNLOAD_ROUTE.getValue()) return DOWNLOAD_ROUTE;
            else if (value == CONFIGURE_WIFI.getValue()) return CONFIGURE_WIFI;
            else if (value == SENSORS_LOGGER.getValue()) return SENSORS_LOGGER;
            else if (value == CALIBRATION_SETTINGS.getValue()) return CALIBRATION_SETTINGS;
            else if (value == CONTROL_SETTINGS.getValue()) return CONTROL_SETTINGS;
            else if (value == ROUTE_CONTAINER.getValue()) return ROUTE_CONTAINER;
            else if (value == CALIBRATION_SETTINGS_DATA.getValue()) return CALIBRATION_SETTINGS_DATA;
            else if (value == CONTROL_SETTINGS_DATA.getValue()) return CONTROL_SETTINGS_DATA;
            else if (value == ROUTE_CONTAINER_DATA.getValue()) return ROUTE_CONTAINER_DATA;
            else if (value == PING_VALUE.getValue()) return PING_VALUE;
            else if (value == WIFI_CONFIGURATION.getValue()) return WIFI_CONFIGURATION;
            else if (value == WIFI_CONFIGURATION_DATA.getValue()) return WIFI_CONFIGURATION_DATA;
            else if (value == WHO_AM_I_VALUE.getValue()) return WHO_AM_I_VALUE;
            else if (value == PROTOCOL_VERSION_VALUE.getValue()) return PROTOCOL_VERSION_VALUE;
            else if (value == PROTOCOL_VERSION.getValue()) return PROTOCOL_VERSION;
            else return DUMMY; // TODO throw some exception
        }
    }

    public enum Parameter {
        DUMMY_PARAMETER(0),

        START(1000011),
        ACK(1000012),
        DATA_ACK(1000013),
        ENTER_DFU(1000014),

        BREAK(1000015),
        BREAK_ACK(1000016),
        BREAK_FAIL(1000017),

        DONE(1000018),
        READY(1000019),
        FAIL(1000020),
        SKIP(1000021),

        NON_STATIC(1000022),
        NOT_ALLOWED(1000023),

        DATA_INVALID(1000024),
        TIMEOUT(1000025),

        VIA_ROUTE_ALLOWED(1000026),
        VIA_ROUTE_NOT_ALLOWED(1000027);

        private final int value;

        Parameter(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        static public Parameter getParameter(int value) {
            if (value == DUMMY_PARAMETER.getValue()) return DUMMY_PARAMETER;
            else if (value == START.getValue()) return START;
            else if (value == ACK.getValue()) return ACK;
            else if (value == DATA_ACK.getValue()) return DATA_ACK;
            else if (value == ENTER_DFU.getValue()) return ENTER_DFU;
            else if (value == BREAK.getValue()) return BREAK;
            else if (value == BREAK_ACK.getValue()) return BREAK_ACK;
            else if (value == BREAK_FAIL.getValue()) return BREAK_FAIL;
            else if (value == DONE.getValue()) return DONE;
            else if (value == READY.getValue()) return READY;
            else if (value == FAIL.getValue()) return FAIL;
            else if (value == SKIP.getValue()) return SKIP;
            else if (value == NON_STATIC.getValue()) return NON_STATIC;
            else if (value == NOT_ALLOWED.getValue()) return NOT_ALLOWED;
            else if (value == DATA_INVALID.getValue()) return DATA_INVALID;
            else if (value == TIMEOUT.getValue()) return TIMEOUT;
            else if (value == VIA_ROUTE_ALLOWED.getValue()) return VIA_ROUTE_ALLOWED;
            else if (value == VIA_ROUTE_NOT_ALLOWED.getValue()) return VIA_ROUTE_NOT_ALLOWED;
            else return DUMMY_PARAMETER; // TODO throw some exception
        }
    }
}
