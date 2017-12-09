package com.skydive.java.events;

import com.skydive.java.CommMessage;
import com.skydive.java.data.SignalData;

/**
 * Created by NawBar on 2016-10-12.
 */
public abstract class CommEvent {
    public enum EventType {
        MESSAGE_RECEIVED,
        SIGNAL_PAYLOAD_RECEIVED,
    }

    public abstract EventType getType();

    @Override
    public String toString() {
        return getType().toString();
    }

    public SignalData getSignalData() throws Exception {
        if (getType() == CommEvent.EventType.MESSAGE_RECEIVED
                && ((MessageEvent) this).getMessageType() == CommMessage.MessageType.SIGNAL) {
            return new SignalData(((MessageEvent) this).getMessage());
        } else {
            throw new Exception("Can not get SignalData form event, bad event used!");
        }
    }

    public boolean matchSignalData(SignalData command) throws Exception {
        return getSignalData().equals(command);
    }
}
