package com.skydive.java.data;

import com.skydive.java.CommMessage;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by Bartosz Nawrot on 2016-10-14.
 */
public class RouteContainer implements SignalPayloadData {

    static public class Waypoint {
        double latitude, longitude;
        float absoluteAltitude;
        float relativeAltitude;
        float velocity;

        public double getLatitude() {
            return latitude;
        }

        public double getLongitude() {
            return longitude;
        }

        public float getAbsoluteAltitude() {
            return absoluteAltitude;
        }

        public float getRelativeAltitude() {
            return relativeAltitude;
        }

        public float getVelocity() {
            return velocity;
        }

        public Waypoint(double lat, double lon, float absAlt, float relAlt, float vel) {
            latitude = lat;
            longitude = lon;
            absoluteAltitude = absAlt;
            relativeAltitude = relAlt;
            velocity = vel;
        }

        Waypoint(final byte[] dataArray) {
            ByteBuffer buffer = ByteBuffer.wrap(dataArray);
            buffer.order(ByteOrder.LITTLE_ENDIAN);

            latitude = buffer.getDouble();
            longitude = buffer.getDouble();
            absoluteAltitude = buffer.getFloat();
            relativeAltitude = buffer.getFloat();
            velocity = buffer.getFloat();
        }

        byte[] serialize() {
            ByteBuffer buffer = ByteBuffer.allocate(getWaypointDataSize());
            buffer.order(ByteOrder.LITTLE_ENDIAN);

            buffer.putDouble(latitude);
            buffer.putDouble(longitude);
            buffer.putFloat(absoluteAltitude);
            buffer.putFloat(relativeAltitude);
            buffer.putFloat(velocity);

            return buffer.array();
        }

        @Override
        public String toString() {
            return "latitude: " + latitude
                    + ", longitude: " + longitude
                    + ", absolute: " + absoluteAltitude
                    + ", relative: " + relativeAltitude
                    + ", velocity: " + velocity;
        }
    }

    private int crcValue;
    private int routeSize;
    private float waypointTime; // [s]
    private float baseTime; // [s]

    private ArrayList<Waypoint> route;

    public RouteContainer() {
        routeSize = 0;
        waypointTime = 10;
        baseTime = 20;
        route = new ArrayList<>();
        crcValue = computeCrc();
    }

    public RouteContainer(final byte[] dataArray) {
        route = new ArrayList<>();

        ByteBuffer buffer = ByteBuffer.wrap(dataArray);
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        crcValue = buffer.getInt();
        routeSize = buffer.getInt();
        waypointTime = buffer.getFloat();
        baseTime = buffer.getFloat();

        int position = 16;
        byte[] waypointData = new byte[getWaypointDataSize()];
        for (int i = 0; i < routeSize; i++) {
            System.arraycopy(buffer.array(), position, waypointData, 0, getWaypointDataSize());
            route.add(new Waypoint(waypointData));
            position += getWaypointDataSize();
        }
    }

    @Override
    public SignalData.Command getDataCommand() {
        return SignalData.Command.ROUTE_CONTAINER;
    }

    @Override
    public SignalData.Command getDataType() {
        return SignalData.Command.ROUTE_CONTAINER_DATA;
    }

    private int computeCrc() {
        // compute CRC value from whole data, excluding last 4 bytes (CRC value)
        return CommMessage.computeCrc32(Arrays.copyOfRange(serialize(), 4, getDataArraySize()));
    }

    public boolean isValid() {
        return crcValue == computeCrc();
    }

    @Override
    public ArrayList<CommMessage> getMessages() {
        return CommMessage.buildMessagesList(getDataType(), serialize());
    }

    private static int getConstraintDataSize() {
        return 16;
    }

    private static int getWaypointDataSize() {
        return 28;
    }

    private int getDataArraySize() {
        // whole route container size is "Constraint"(16b) + routeSize*"Waypoint"(28b)
        return getConstraintDataSize() + routeSize * getWaypointDataSize();
    }

    public byte[] serialize() {
        byte[] dataArray = new byte[getDataArraySize()];
        ByteBuffer buffer = ByteBuffer.allocate(getConstraintDataSize());
        buffer.order(ByteOrder.LITTLE_ENDIAN);

        // constraint
        buffer.putInt(crcValue);
        buffer.putInt(routeSize);
        buffer.putFloat(waypointTime);
        buffer.putFloat(baseTime);
        // put constraint in to array
        System.arraycopy(buffer.array(), 0, dataArray, 0, getConstraintDataSize());
        // serialize route
        if (routeSize != 0) {
            int position = getConstraintDataSize();
            for (Waypoint w : route) {
                byte[] waypointData = w.serialize();
                System.arraycopy(waypointData, 0, dataArray, position, getWaypointDataSize());
                position += getWaypointDataSize();
            }
        }

        return dataArray;
    }

    public ArrayList<Waypoint> getRoute() {
        return route;
    }

    public void resetRoute() {
        route.clear();
        routeSize = 0;
    }

    public void addWaypoint(Waypoint waypoint) {
        route.add(waypoint);
        routeSize++;
    }

    public float getWaypointTime() {
        return waypointTime;
    }

    public float getBaseTime() {
        return baseTime;
    }

    public int getRouteSize() {
        return routeSize;
    }

    public int getCrcValue() {
        return crcValue;
    }

    public void setWaypointTime(float waypointTime) {
        this.waypointTime = waypointTime;
    }

    public void setBaseTime(float baseTime) {
        this.baseTime = baseTime;
    }

    public void setCrc() {
        this.crcValue = computeCrc();
    }
}
