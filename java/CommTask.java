package com.skydive.java;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Bartosz Nawrot on 2016-08-30.
 * Abstract class for all synchronous communication tasks.
 * Controlled by start and stop methods, dynamically responds for frequency change.
 */
public abstract class CommTask {
    private Timer timer;

    // frequency of task [Hz]
    private double frequency;

    private boolean isRunning;

    protected CommTask(double frequency) {
        this.frequency = frequency;
        this.isRunning = false;
    }

    public void start() {
        start(frequency);
    }

    private void start(double freq) {
        System.out.println("Starting task: " + getTaskName() + " with freq: " + String.valueOf(freq) + "Hz");
        timer = new Timer(getTaskName() + "_timer");
        TimerTask timerTask = new TimerTask() {
            @Override
            public void run() {
                task();
            }
        };
        long period = (long) ((1.0 / freq) * 1000);
        long delay = period > 200 ? period : 200;
        System.out.println("Starting " + getTaskName() + " task with freq: " + String.valueOf(freq) + " Hz, and delay: " + String.valueOf(delay) + " ms");
        timer.scheduleAtFixedRate(timerTask, delay, period);
        isRunning = true;
        onStarted();
    }

    public void stop() {
        System.out.println("Stopping task: " + getTaskName());
        if (timer != null) {
            timer.cancel();
        }
        isRunning = false;
        onStopped();
    }

    public void setFrequency(double frequency) {
        this.frequency = frequency;
        if (isRunning) {
            stop();
            start();
        }
    }

    protected abstract String getTaskName();

    protected abstract void task();

    protected void onStarted() {
        // nothing to do here, user can override
    }

    protected void onStopped() {
        // nothing to do here, user can override
    }
}
