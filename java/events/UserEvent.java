package com.skydive.java.events;

import com.skydive.java.actions.CommHandlerAction;

/**
 * Created by nawbar on 2016-10-14.
 * This class contains events that can be postd by user action from UI
 * to control specific communication actions. This class is not accessible from outsige package.
 * External events should be posted by proper methods called on UavManager.
 */

public class UserEvent {
    public enum Type {
        END_FLIGHT_LOOP,
        DONE_MAGNETOMETER_CALIBRATION,
        CANCEL_MAGNETOMETER_CALIBRATION,
    }

    private Type type;

    public UserEvent(Type type) {
        this.type = type;
    }

    public Type getType() {
        return type;
    }

    public CommHandlerAction.ActionType getOwnerAction() {
        switch (type) {
            case END_FLIGHT_LOOP:
                return CommHandlerAction.ActionType.FLIGHT_LOOP;
            case DONE_MAGNETOMETER_CALIBRATION:
                return CommHandlerAction.ActionType.MAGNETOMETER_CALIBRATION;
            case CANCEL_MAGNETOMETER_CALIBRATION:
                return CommHandlerAction.ActionType.MAGNETOMETER_CALIBRATION;
            default:
                // TODO error, shall never reach this place
                return CommHandlerAction.ActionType.IDLE;
        }
    }
}
