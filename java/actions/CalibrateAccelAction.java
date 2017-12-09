package com.skydive.java.actions;

import com.skydive.java.CommHandler;
import com.skydive.java.UavEvent;
import com.skydive.java.data.CalibrationSettings;
import com.skydive.java.data.DebugData;
import com.skydive.java.data.SignalData;
import com.skydive.java.events.CommEvent;
import com.skydive.java.events.MessageEvent;
import com.skydive.java.events.SignalPayloadEvent;

/**
 * Created by Bartosz Nawrot on 2016-10-22.
 */

public class CalibrateAccelAction extends CommHandlerAction {

    public enum CalibrationState {
        IDLE,
        INITIAL_COMMAND,
        WAITING_FOR_CALIBRATION,
        WAITING_FOR_CALIBRATION_DATA
    }

    private CalibrationState state;

    private boolean calibrationProcedureDone;

    public CalibrateAccelAction(CommHandler commHandler) {
        super(commHandler);
        state = CalibrationState.IDLE;
        calibrationProcedureDone = false;
    }

    @Override
    public boolean isActionDone() {
        return calibrationProcedureDone;
    }

    @Override
    public void start() {
        System.out.println("Starting calibration accelerometer procedure");
        calibrationProcedureDone = false;
        state = CalibrationState.INITIAL_COMMAND;
        commHandler.stopCommTask(commHandler.getPingTask());
        commHandler.send(new SignalData(SignalData.Command.CALIBRATE_ACCEL, SignalData.Parameter.START).getMessage());
    }

    @Override
    public void handleEvent(CommEvent event) throws Exception {
        CalibrationState actualState = state;
        switch (state) {
            case INITIAL_COMMAND:
                if (event.getType() == CommEvent.EventType.MESSAGE_RECEIVED) {
                    switch (((MessageEvent) event).getMessageType()) {
                        case CONTROL:
                            System.out.println("DebugData received when waiting for ACK on initial calibrate accelerometer command");
                            commHandler.getUavManager().setDebugData(new DebugData(((MessageEvent) event).getMessage()));
                            break;

                        case SIGNAL:
                            if (event.matchSignalData(new SignalData(SignalData.Command.CALIBRATE_ACCEL, SignalData.Parameter.ACK))) {
                                System.out.println("Accelerometer calibration starts");
                                state = CalibrationState.WAITING_FOR_CALIBRATION;
                            } else {
                                System.out.println("Unexpected event received at state " + state.toString());
                            }
                            break;
                    }
                }
                break;

            case WAITING_FOR_CALIBRATION:
                if (event.matchSignalData(
                        new SignalData(SignalData.Command.CALIBRATE_ACCEL, SignalData.Parameter.DONE))) {
                    state = CalibrationState.WAITING_FOR_CALIBRATION_DATA;
                    System.out.println("Calibration done successfully, data ready");
                } else if (event.matchSignalData(
                        new SignalData(SignalData.Command.CALIBRATE_ACCEL, SignalData.Parameter.NON_STATIC))) {
                    System.out.println("Calibration non static");
                    commHandler.getUavManager().notifyUavEvent(new UavEvent(UavEvent.Type.MESSAGE, "Accelerometer calibration non static!"));
                    calibrationProcedureDone = true;
                    commHandler.notifyActionDone();
                } else {
                    System.out.println("Unexpected event received at state " + state.toString());
                }
                break;

            case WAITING_FOR_CALIBRATION_DATA:
                if (event.getType() == CommEvent.EventType.SIGNAL_PAYLOAD_RECEIVED
                        && ((SignalPayloadEvent) event).getDataType() == SignalData.Command.CALIBRATION_SETTINGS_DATA) {
                    SignalPayloadEvent signalEvent = (SignalPayloadEvent) event;

                    CalibrationSettings calibrationSettings = (CalibrationSettings) signalEvent.getData();
                    if (calibrationSettings.isValid()) {
                        System.out.println("Calibration settings received after accelerometer calibration");
                        commHandler.send(new SignalData(SignalData.Command.CALIBRATION_SETTINGS, SignalData.Parameter.ACK).getMessage());
                        commHandler.getUavManager().setCalibrationSettings(calibrationSettings);
                        commHandler.getUavManager().notifyUavEvent(new UavEvent(UavEvent.Type.MESSAGE, "Accelerometer calibration successful"));
                        calibrationProcedureDone = true;
                        commHandler.notifyActionDone();
                        commHandler.getUavManager().notifyUavEvent(new UavEvent((UavEvent.Type.ACCEL_CALIB_DONE)));
                    } else {
                        System.out.println("Calibration settings received but the data is invalid, responding with DATA_INVALID");
                        commHandler.send(new SignalData(SignalData.Command.CALIBRATION_SETTINGS, SignalData.Parameter.DATA_INVALID).getMessage());
                    }
                } else {
                    System.out.println("Unexpected event received at state " + state.toString());
                }
                break;

            default:
                throw new Exception("Event: " + event.toString() + " received at unknown state");
        }
        if (actualState != state) {
            System.out.println("HandleEvent done, transition: " + actualState.toString() + " -> " + state.toString());
        } else {
            System.out.println("HandleEvent done, no state change");
        }
    }

    @Override
    public ActionType getActionType() {
        return ActionType.ACCELEROMETER_CALIBRATION;
    }
}