#pragma once

#include <array>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

extern "C" {
#include <linux/joystick.h>
}

enum class Button {
    Trigger = 0,
    Thumb = 1,
    Thumb2 = 2,
    Top = 3,
    Top2 = 4,
    Pinkie = 5,
    Base = 6,
    Base2 = 7,
    Base3 = 8,
    Base4 = 9,
    Base5 = 10,
    Base6 = 11,
    Dead = 12
};

struct ControlData {
    float x = 0.0f;
    float y = 0.0f;
    float twist = 0.0f;

    std::array<bool, 13> buttons{};

    bool isPressed(Button b) const {
        return buttons[static_cast<int>(b)];
    }
};

class Joystick {
public:
    explicit Joystick(const char* device = "/dev/input/js0");
    ~Joystick();

    void start();
    void stop();

    ControlData getLatestData();

    void setCallback(std::function<void(const ControlData&)> cb);

private:
    void run();

    int fd = -1;
    std::thread worker;
    std::atomic<bool> running{false};

    float axis[8] = {0};
    bool buttonState[13] = {false};

    ControlData latestData;
    std::mutex dataMutex;

    std::function<void(const ControlData&)> callback;

    float normalize(int value);
    float applyDeadzone(float v);
};