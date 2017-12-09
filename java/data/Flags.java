package com.skydive.java.data;

/**
 * Created by Bartosz Nawrot on 2016-09-02.
 */
public class Flags {

    final private int size;
    private int flags;


    public Flags(int size) {
        this.size = size;
        this.flags = 0;
    }

    public Flags(int size, int initial) {
        this.size = size;
        this.flags = initial;
    }

    void setFlagsState(int id, boolean state) {
        if (id > size) {
            return;
        }
        if (state) {
            flags |= 1 << id;
        } else {
            flags &= ~(1 << id);
        }
    }

    boolean getFlagState(int id) {
        return id <= size && (flags & (1 << id)) != 0;
    }

    public int getSize() {
        return size;
    }

    public int getFlags() {
        return flags;
    }

    public void setFlags(int flag) {
        this.flags = flag;
    }
}
