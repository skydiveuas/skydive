package com.skydive.java.actions;

import com.skydive.java.CommHandler;
import com.skydive.java.UavEvent;
import com.skydive.java.data.DebugData;
import com.skydive.java.data.SignalData;
import com.skydive.java.data.SignalPayloadData;
import com.skydive.java.events.CommEvent;
import com.skydive.java.events.MessageEvent;

/**
 * Created by nawba on 12.01.2017.
 */

public class UploadRouteContainerAction extends CommHandlerAction {

    public enum UploadState {
        IDLE,
        INITIAL_COMMAND,
        UPLOADING_DATA

    }

    private UploadState state;
    private boolean uploadProcedureDone;
    private SignalPayloadData routeContainerToUpload;

    public UploadRouteContainerAction(CommHandler commHandler, SignalPayloadData routeContainerToUpload) {
        super(commHandler);
        this.state = UploadState.IDLE;
        this.routeContainerToUpload = routeContainerToUpload;
        uploadProcedureDone = false;
    }

    @Override
    public void start() {
        state = UploadState.INITIAL_COMMAND;
        commHandler.stopCommTask(commHandler.getPingTask());
        commHandler.send(new SignalData(SignalData.Command.UPLOAD_ROUTE, SignalData.Parameter.START).getMessage());
    }

    @Override
    public boolean isActionDone() {
        return uploadProcedureDone;
    }

    @Override
    public void handleEvent(CommEvent event) throws Exception {
        UploadState actualState = state;
        switch (state) {
            case INITIAL_COMMAND:
                if (event.getType() == CommEvent.EventType.MESSAGE_RECEIVED) {
                    switch (((MessageEvent) event).getMessageType()) {
                        case CONTROL:
                            System.out.println("DebugData received when waiting for ACK on RouteContainer upload procedure");
                            commHandler.getUavManager().setDebugData(new DebugData(((MessageEvent) event).getMessage()));
                            break;
                        case SIGNAL:
                            if (event.matchSignalData(new SignalData(SignalData.Command.UPLOAD_ROUTE, SignalData.Parameter.ACK))) {
                                System.out.println("Starting Route Container upload procedure");
                                state = UploadState.UPLOADING_DATA;
                                commHandler.send(routeContainerToUpload);
                            } else {
                                System.out.println("Unexpected event received at state " + state.toString());
                            }
                            break;
                    }
                }
                break;
            case UPLOADING_DATA:
                if (event.matchSignalData(new SignalData(SignalData.Command.ROUTE_CONTAINER, SignalData.Parameter.ACK))) {
                    System.out.println("Route Container settings uploaded");
                    commHandler.getUavManager().notifyUavEvent(new UavEvent(UavEvent.Type.MESSAGE, "Route Container settings uploaded successfully!"));
                    uploadProcedureDone = true;
                    commHandler.notifyActionDone();
                } else if (event.matchSignalData(new SignalData(SignalData.Command.ROUTE_CONTAINER, SignalData.Parameter.DATA_INVALID))) {
                    commHandler.getUavManager().notifyUavEvent(new UavEvent(UavEvent.Type.MESSAGE, "Uploading Route Container settings failed!"));
                    System.out.println("Route Container settings sent but the data is invalid, responding with DATA_INVALID");
                    commHandler.send(routeContainerToUpload);
                } else if (event.matchSignalData(new SignalData(SignalData.Command.ROUTE_CONTAINER, SignalData.Parameter.TIMEOUT))) {
                    commHandler.getUavManager().notifyUavEvent(new UavEvent(UavEvent.Type.MESSAGE, "Uploading Route Container settings timeout!"));
                    System.out.println("Uploading Route Container procedure timeout, responding with TIMEOUT");
                    commHandler.send(routeContainerToUpload);
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
        return ActionType.UPLOAD_ROUTE_CONTAINER;
    }
}
