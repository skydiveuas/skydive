package com.skydive.java.actions;

import com.skydive.java.CommHandler;
import com.skydive.java.data.ControlSettings;
import com.skydive.java.data.DebugData;
import com.skydive.java.data.SignalData;
import com.skydive.java.events.CommEvent;
import com.skydive.java.events.MessageEvent;
import com.skydive.java.events.SignalPayloadEvent;

/**
 * Created by nawbar on 12.01.2017.
 */

public class DownloadControlSettingsAction extends CommHandlerAction {

    private enum DownloadState {
        IDLE,
        INITIAL_COMMAND,
        WAITING_FOR_CONTROL_SETTINGS_DATA
    }

    private DownloadState state;

    private boolean downloadProcedureDone;

    public DownloadControlSettingsAction(CommHandler commHandler) {
        super(commHandler);
        this.state = DownloadState.IDLE;
        downloadProcedureDone = false;

    }

    @Override
    public void start() {
        System.out.println("Starting download control settings procedre");
        state = DownloadState.INITIAL_COMMAND;
        commHandler.stopCommTask(commHandler.getPingTask());
        commHandler.send(new SignalData(SignalData.Command.DOWNLOAD_SETTINGS, SignalData.Parameter.START).getMessage());
    }

    @Override
    public boolean isActionDone() {
        return downloadProcedureDone;
    }

    @Override
    public void handleEvent(CommEvent event) throws Exception {
        DownloadState actualState = state;
        switch (state) {
            case INITIAL_COMMAND:
                if (event.getType() == CommEvent.EventType.MESSAGE_RECEIVED) {
                    switch (((MessageEvent) event).getMessageType()) {
                        case CONTROL:
                            System.out.println("DebugData received when waiting for ACK on initial download control settings command");
                            commHandler.getUavManager().setDebugData(new DebugData(((MessageEvent) event).getMessage()));
                            break;

                        case SIGNAL:
                            if (event.matchSignalData(new SignalData(SignalData.Command.DOWNLOAD_SETTINGS, SignalData.Parameter.ACK))) {
                                System.out.println("Downloading control settings starts");
                                state = DownloadState.WAITING_FOR_CONTROL_SETTINGS_DATA;
                            } else {
                                System.out.println("Unexpected event received at state " + state.toString());
                            }
                            break;
                    }
                }
                break;

            case WAITING_FOR_CONTROL_SETTINGS_DATA:
                if (event.getType() == CommEvent.EventType.SIGNAL_PAYLOAD_RECEIVED
                        && ((SignalPayloadEvent) event).getDataType() == SignalData.Command.CONTROL_SETTINGS_DATA) {
                    SignalPayloadEvent signalEvent = (SignalPayloadEvent) event;

                    ControlSettings controlSettings = (ControlSettings) signalEvent.getData();
                    if (controlSettings.isValid()) {
                        System.out.println("Control settings received");
                        commHandler.send(new SignalData(SignalData.Command.CONTROL_SETTINGS, SignalData.Parameter.ACK).getMessage());
                        commHandler.getUavManager().setControlSettings(controlSettings);
                        downloadProcedureDone = true;
                        commHandler.notifyActionDone();
                    } else {
                        System.out.println("Control settings received but the data is invalid, responding with DATA_INVALID");
                        commHandler.send(new SignalData(SignalData.Command.CONTROL_SETTINGS, SignalData.Parameter.DATA_INVALID).getMessage());
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
        return ActionType.DOWNLOAD_CONTROL_SETTINGS;
    }
}
