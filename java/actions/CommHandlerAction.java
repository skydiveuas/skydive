package com.skydive.java.actions;

import com.skydive.java.CommHandler;
import com.skydive.java.events.CommEvent;
import com.skydive.java.events.UserEvent;

/**
 * Created by NawBar on 2016-10-12.
 */
public abstract class CommHandlerAction {

    protected CommHandler commHandler;

    public enum ActionType {
        IDLE,
        CONNECT,
        DISCONNECT,
        APPLICATION_LOOP,
        FLIGHT_LOOP,
        ACCELEROMETER_CALIBRATION,
        MAGNETOMETER_CALIBRATION,
        UPLOAD_CONTROL_SETTINGS,
        DOWNLOAD_CONTROL_SETTINGS,
        UPLOAD_ROUTE_CONTAINER,
        DOWNLOAD_ROUTE_CONTAINER,
        CALIBRATE_MAGNETOMETER;
    }

    CommHandlerAction(CommHandler commHandler) {
        this.commHandler = commHandler;
    }

    public abstract boolean isActionDone();

    public abstract void start();

    public abstract void handleEvent(CommEvent event) throws Exception;

    public void notifyUserEvent(UserEvent userEvent) {
        System.out.println("User event handled by base handler, no action");
    }

    public abstract ActionType getActionType();

    public String getActionName() {
        return getActionType().toString();
    }

    @Override
    public String toString() {
        return getActionName();
    }
}