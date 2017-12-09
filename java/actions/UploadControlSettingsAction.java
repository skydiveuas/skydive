package com.skydive.java.actions;

import com.skydive.java.CommHandler;
import com.skydive.java.UavEvent;
import com.skydive.java.data.DebugData;
import com.skydive.java.data.SignalData;
import com.skydive.java.data.SignalPayloadData;
import com.skydive.java.events.CommEvent;
import com.skydive.java.events.MessageEvent;

/**
 * Created by nawbar on 12.01.2017.
 */

public class UploadControlSettingsAction extends CommHandlerAction {

    public enum UploadState {
        IDLE,
        INITIAL_COMAND,
        WAITING_FOR_ACK

    }

    private UploadState state;

    private boolean uploadProcedureDone;

    private SignalPayloadData controlSettingsToUpload;

    public UploadControlSettingsAction(CommHandler commHandler, SignalPayloadData controlSettingsToUpload) {
        super(commHandler);
        this.state = UploadState.IDLE;
        this.controlSettingsToUpload = controlSettingsToUpload;
        uploadProcedureDone = false;
    }

    @Override
    public void start() {
        System.out.println("Starting upload control settings procedure");
        state = UploadState.INITIAL_COMAND;
        commHandler.stopCommTask(commHandler.getPingTask());
        commHandler.send(new SignalData(SignalData.Command.UPLOAD_SETTINGS, SignalData.Parameter.START).getMessage());

    }

    @Override
    public boolean isActionDone() {
        return uploadProcedureDone;
    }

    @Override
    public void handleEvent(CommEvent event) throws Exception {
        UploadState actualState = state;
        switch (state) {
            case INITIAL_COMAND:
                if (event.getType() == CommEvent.EventType.MESSAGE_RECEIVED) {
                    switch (((MessageEvent) event).getMessageType()) {
                        case CONTROL:
                            System.out.println("DebugData received when waiting for ACK on initial upload control settings command");
                            commHandler.getUavManager().setDebugData(new DebugData(((MessageEvent) event).getMessage()));
                            break;

                        case SIGNAL:
                            if (event.matchSignalData(new SignalData(SignalData.Command.UPLOAD_SETTINGS, SignalData.Parameter.ACK))) {
                                System.out.println("Uploading control settings starts");
                                commHandler.send(controlSettingsToUpload);
                                state = UploadState.WAITING_FOR_ACK;
                            } else {
                                System.out.println("Unexpected event received at state " + state.toString());
                            }
                            break;
                    }
                }
                break;

            case WAITING_FOR_ACK:
                if (event.matchSignalData(new SignalData(SignalData.Command.CONTROL_SETTINGS, SignalData.Parameter.ACK))) {
                    System.out.println("Control settings uploaded");
                    commHandler.getUavManager().notifyUavEvent(new UavEvent(UavEvent.Type.MESSAGE, "Control settings uploaded successfully!"));
                    uploadProcedureDone = true;
                    commHandler.notifyActionDone();
                } else if (event.matchSignalData(new SignalData(SignalData.Command.CONTROL_SETTINGS, SignalData.Parameter.DATA_INVALID))
                        || event.matchSignalData(new SignalData(SignalData.Command.CONTROL_SETTINGS, SignalData.Parameter.TIMEOUT))) {
                    System.out.println("Uploading Control Settings failed!");
                    commHandler.send(controlSettingsToUpload);
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
        return ActionType.UPLOAD_CONTROL_SETTINGS;
    }
}
