package com.skydive.java.events;

import com.skydive.java.data.SignalData;
import com.skydive.java.data.SignalPayloadData;

/**
 * Created by nawba on 15.10.2016.
 */
public class SignalPayloadEvent extends CommEvent {

    private SignalPayloadData data;

    public SignalPayloadEvent(SignalPayloadData data) {
        this.data = data;
    }

    public SignalData.Command getDataType() {
        return data.getDataType();
    }

    public SignalPayloadData getData() {
        return data;
    }

    @Override
    public EventType getType() {
        return EventType.SIGNAL_PAYLOAD_RECEIVED;
    }

    @Override
    public String toString() {
        return "SIGNAL_PAYLOAD_EVENT: " + getDataType().toString();
    }
}