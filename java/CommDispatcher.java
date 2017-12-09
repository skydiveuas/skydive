package com.skydive.java;

import com.skydive.java.data.*;
import com.skydive.java.events.CommEvent;
import com.skydive.java.events.MessageEvent;
import com.skydive.java.events.SignalPayloadEvent;

/**
 * Created by nawba on 16.10.2016.
 */
public class CommDispatcher {

    private byte[] preambleBuffer;
    private byte[] dataBuffer;

    private int preambleBufferCounter;
    private int dataBufferCounter;

    private boolean isPreambleActive;
    private CommMessage.MessageType activePreambleType;

    private int targetDataBufferCounter;

    private int failedReceptionCounter;
    private int successfulReceptionCounter;

    private boolean receivingSignalData;
    private SignalData.Command receivedSignalData;
    private byte[] signalDataBuffer;
    private int signalDataPacketsToReceive;
    private int signalDataPacketsReceived;

    private CommDispatcherListener listener;

    public CommDispatcher(CommDispatcherListener listener) {
        this.listener = listener;
        reset();
    }

    public void proceedReceiving(final byte[] data, final int dataSize) {
        for (int i = 0; i < dataSize; i++) {
            proceedByte(data[i]);
        }
    }

    public void reset() {
        preambleBuffer = new byte[CommMessage.PREAMBLE_SIZE - 1];
        dataBuffer = new byte[CommMessage.MAX_MESSAGE_SIZE];

        preambleBufferCounter = 0;
        dataBufferCounter = 0;

        isPreambleActive = false;
        activePreambleType = CommMessage.MessageType.EMPTY;

        targetDataBufferCounter = 0;

        failedReceptionCounter = 0;
        successfulReceptionCounter = 0;

        receivingSignalData = false;
        receivedSignalData = SignalData.Command.DUMMY;
        signalDataBuffer = null;
        signalDataPacketsToReceive = 0;
        signalDataPacketsReceived = 0;
    }

    private void proceedByte(final byte b) {
        // check for new preamble
        final CommMessage.MessageType newPreamble = updatePreamble(b);
        if (newPreamble != CommMessage.MessageType.EMPTY) {
            if (isPreambleActive) {
                System.out.println("SkyDive::CommDispatcher::FAIL: new preamble " +
                        "received when previous reception not ready");
                failedReceptionCounter++;
            }
            activatePreamble(newPreamble);
            return;
        }

        if (isPreambleActive) {
            // proceed processing data
            dataBuffer[dataBufferCounter] = b;
            dataBufferCounter++;

            if (dataBufferCounter >= targetDataBufferCounter) {
                // enough data in data buffer
                // check signal message condition
                if (activePreambleType == CommMessage.MessageType.SIGNAL
                        && dataBufferCounter == CommMessage.SIGNAL_CONSTRAINT_SIZE) {
                    // command from signal message just received, update target
                    updateTargetDataSizeWithCommand();
                    return;
                }

                // check CRC condition
                if (isValidMessageCrc()) {
                    // data received successfully!
                    successfulReceptionCounter++;
                    if (activePreambleType == CommMessage.MessageType.SIGNAL &&
                            SignalData.hasPayload(SignalData.parseCommand(dataBuffer))) {
                        handleSignalDataPayloadReception();
                        if (isSignalDataComplete()) {
                            receivingSignalData = false;
                            // signal data payload received successfully
                            listener.handleCommEvent(
                                    new SignalPayloadEvent(signalPayloadMessageFactory()));
                        }
                    } else {
                        if (receivingSignalData) {
                            System.out.println("SkyDive::CommDispatcher::FAIL: receiving SignalData not ready");
                            failedReceptionCounter++;
                        }
                        receivingSignalData = false;
                        // preamble message payload received successfully
                        listener.handleCommEvent(
                                new MessageEvent(new CommMessage(activePreambleType, dataBuffer)));
                    }
                } else {
                    // something gone wrong, reset processor
                    System.out.println("SkyDive::CommDispatcher::FAIL: wrong CRC");
                    failedReceptionCounter++;
                }
                deactivatePreamble();
            }
        }
    }

    private CommMessage.MessageType updatePreamble(final byte b) {
        // new preamble is arrived when all preamble bytes are the same
        // and equals to known preamble type
        // and the last received byte is equal to 0
        CommMessage.MessageType result = CommMessage.MessageType.EMPTY;
        if (b == 0) {
            boolean allEquals = true;
            for (byte by : preambleBuffer) {
                if (preambleBuffer[0] != by) {
                    allEquals = false;
                    break;
                }
            }
            if (allEquals) {
                result = CommMessage.getPreambleTypeByChar(preambleBuffer[0]);
            }
        }
        preambleBuffer[preambleBufferCounter] = b;
        preambleBufferCounter++;
        if (preambleBufferCounter >= CommMessage.PREAMBLE_SIZE - 1) {
            preambleBufferCounter = 0;
        }
        return result;
    }

    private void activatePreamble(final CommMessage.MessageType preambleType) {
        isPreambleActive = true;
        activePreambleType = preambleType;

        dataBufferCounter = 0;
        targetDataBufferCounter = CommMessage.getPayloadSizeByType(preambleType);
        if (activePreambleType != CommMessage.MessageType.SIGNAL) {
            targetDataBufferCounter += CommMessage.CRC_SIZE;
        }
    }

    private void updateTargetDataSizeWithCommand() {
        final SignalData.Command command = SignalData.parseCommand(dataBuffer);
        if (SignalData.hasPayload(command)) {
            targetDataBufferCounter += CommMessage.SIGNAL_DATA_PAYLOAD_SIZE + CommMessage.CRC_SIZE;
        } else {
            targetDataBufferCounter += CommMessage.CRC_SIZE;
        }
    }

    private boolean isValidMessageCrc() {
        byte[] tmpArray = new byte[targetDataBufferCounter - CommMessage.CRC_SIZE];
        System.arraycopy(dataBuffer, 0, tmpArray, 0, tmpArray.length);
        final short crcValue = CommMessage.computeCrc16(tmpArray);
        byte crc1 = (byte) (crcValue & 0xff);
        byte crc2 = (byte) ((crcValue >> 8) & 0xff);
        return dataBuffer[targetDataBufferCounter - 2] == crc1
                && dataBuffer[targetDataBufferCounter - 1] == crc2;
    }

    private void initSignalDataPayloadReception(final SignalData.Command command, final short allPackets) {
        receivedSignalData = command;
        signalDataBuffer = new byte[allPackets * CommMessage.SIGNAL_DATA_PAYLOAD_SIZE];
        signalDataPacketsToReceive = allPackets;
        receivingSignalData = true;
        signalDataPacketsReceived = 0;
    }

    private void handleSignalDataPayloadReception() {
        final SignalData.Command command = SignalData.parseCommand(dataBuffer);
        final short allPackets = SignalData.parseAllPacketsNumber(dataBuffer);
        final short packetNumber = SignalData.parseActualPacketNumber(dataBuffer);

        if (!receivingSignalData || receivedSignalData != command) {
            // new data or another data was being received
            initSignalDataPayloadReception(command, allPackets);
        }
        // put data to buffer in to reported position
        System.arraycopy(dataBuffer, CommMessage.SIGNAL_CONSTRAINT_SIZE,
                signalDataBuffer, packetNumber * CommMessage.SIGNAL_DATA_PAYLOAD_SIZE,
                CommMessage.SIGNAL_DATA_PAYLOAD_SIZE);
        signalDataPacketsReceived++;
    }

    private boolean isSignalDataComplete() {
        return signalDataPacketsToReceive == signalDataPacketsReceived;
    }

    private SignalPayloadData signalPayloadMessageFactory() {
        if (receivedSignalData == SignalData.Command.CALIBRATION_SETTINGS_DATA) {
            return new CalibrationSettings(signalDataBuffer);
        } else if (receivedSignalData == SignalData.Command.CONTROL_SETTINGS_DATA) {
            return new ControlSettings(signalDataBuffer);
        } else if (receivedSignalData == SignalData.Command.ROUTE_CONTAINER_DATA) {
            return new RouteContainer(signalDataBuffer);
        } else {
            // error, wrong data command
            return null;
        }
    }

    private void deactivatePreamble() {
        isPreambleActive = false;
        activePreambleType = CommMessage.MessageType.EMPTY;
        dataBufferCounter = 0;
        targetDataBufferCounter = 0;
    }

    public int getFailedReceptionCounter() {
        return failedReceptionCounter;
    }

    public int getSuccessfulReceptionCounter() {
        return successfulReceptionCounter;
    }

    public interface CommDispatcherListener {
        void handleCommEvent(CommEvent event);
    }
}