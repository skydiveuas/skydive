package com.skydive.java.data;

import com.skydive.java.CommMessage;

import java.util.ArrayList;

/**
 * Created by nawba on 15.10.2016.
 */
public interface SignalPayloadData {

    /**
     * getDataCommand
     */
    SignalData.Command getDataCommand();

    /**
     * getDataType
     */
    SignalData.Command getDataType();

    /**
     * getMessages
     */
    ArrayList<CommMessage> getMessages();
}
