#include "joystick.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <errno.h>

Joystick::Joystick(const char* device)
{
    fd = open(device, O_RDONLY | O_NONBLOCK);

    if (fd < 0) {
        std::cerr << "Joystick açılamadı\n";
    }
}

Joystick::~Joystick() {
    stop();

    if (fd >= 0)
        close(fd);
}

void Joystick::start() {
    if (fd < 0) return;
    if (running) return;

    running = true;
    worker = std::thread(&Joystick::run, this);
}

void Joystick::stop() {
    if (!running) return;

    running = false;

    if (worker.joinable())
        worker.join();
}

float Joystick::normalize(int value) {
    return value / 32767.0f;
}

float Joystick::applyDeadzone(float v) {
    return (std::fabs(v) < 0.05f) ? 0.0f : v;
}

void Joystick::run() {
    struct js_event e;

    while (running) {

        while (true) {
            ssize_t bytes = read(fd, &e, sizeof(e));

            if (bytes > 0) {

                e.type &= ~JS_EVENT_INIT;

                if (e.type == JS_EVENT_AXIS && e.number < 8) {
                    axis[e.number] =
                        applyDeadzone(normalize(e.value));
                }
                else if (e.type == JS_EVENT_BUTTON && e.number < 13) {
                    buttonState[e.number] = (e.value != 0);
                }

            } else {
                if (errno == EAGAIN) {
                    break; 
                } else {
                    std::cerr << "Joystick read error\n";
                    break;
                }
            }
        }

        ControlData data;
        data.x = axis[0];
        data.y = axis[1];
        data.twist = axis[2];

        for (int i = 0; i < 13; i++) {
            data.buttons[i] = buttonState[i];
        }

       
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            latestData = data;
        }

       
        if (callback) {
            callback(data);
        }

        usleep(1000);
    }
}

ControlData Joystick::getLatestData() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return latestData;
}

void Joystick::setCallback(std::function<void(const ControlData&)> cb) {
    callback = cb;
}