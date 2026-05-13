#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "joystick.hpp"

#define LOCK_THIS_SCOPE std::lock_guard<std::mutex> lock(this->data_mutex);

Joystick::Joystick(const char* device_path, AI* ai) : 
        data_listen_flag(true),
        ButtonStates(new bool[16]),
        AxesValues(new short int[4]) {

    this->js_fd = open(device_path, O_RDONLY);
    this->ai = ai;

    if (this->js_fd == -1) {
        std::cerr<<"[ERROR] FROM Joystick::Joystick: "<<device_path<<" has an issue. Exiting.";
        exit(EXIT_FAILURE);
    }

    // Starts to manage recording audio by using `AI::start_recording()` and `AI::stop_recording()`
    this->AI_thread = std::thread([this]{
        while (this->data_listen_flag) {
            if (this->ButtonStates[Button::BUTTON_3]) {
                this->ai->start_recording();
            } else if (this->ai->get_is_recording()) {
                this->ai->stop_recording();
            }
        }
    });

    // Starts thread to listen data. 
    this->listening_thread = std::thread([this]{
        while(this->data_listen_flag){
            long int bytes = read(this->js_fd, &this->event, sizeof(this->event));
            if (bytes >0 ) {
                this->parse_event_data();
            } else {
                std::cerr<<"[ERROR] FROM Joystick::Joystick: Could not read data from Joystick device";
                exit(EXIT_FAILURE);
            }
        }
    });
}

Joystick::~Joystick() {
    this->data_listen_flag = false;
    this->listening_thread.join();
    this->AI_thread.join();

    delete[] this->AxesValues;
    delete[] this->ButtonStates;
}

void Joystick::parse_event_data() {
    int event_type      = this->event.type;     // 0x01: JS_EVENT_BUTTON, 0x02: JS_EVENT_AXIS
    int event_number    = static_cast<int>(this->event.number); // [0-11] for JS_EVENT_BUTTON, [0-5] for JS_EVENT_AXIS
    short int event_val = this->event.value;    // Event's value; For buttons [0, 1], for axes [-32768, 32767]
    
    LOCK_THIS_SCOPE // Locks `parse_event_data()`s entire scope.

    if (event_type == JS_EVENT_BUTTON) {        // If the event comes from joystick is about buttons
        this->ButtonStates[event_number] = event_val;   // Updates button state
    } else if (event_type == JS_EVENT_AXIS) {   // If the event comes from joystick is about axes
        if (event_number <= 2) {        // Event; if it comes from Yaw(0), Pitch(1), Roll(2)
            this->AxesValues[event_number] = event_val;     // Updates axis value and mutex.unlock()
        } else if (event_number == 3) { // Event; if it comes from Throttle(3)
            this->AxesValues[event_number] = -event_val;
        } else if (event_number == 4) { // If data comes from LEFT_RIGHT_BUTTONS(4)
            if (event_val < 0) {
                this->ButtonStates[Button::BUTTON_LEFT]  = true;
            } else if (event_val > 0) {
                this->ButtonStates[Button::BUTTON_RIGHT] = true;
            } else {
                this->ButtonStates[Button::BUTTON_LEFT]  = false;
                this->ButtonStates[Button::BUTTON_RIGHT] = false;
            }
        } else if (event_number == 5) { // If data comes from UP_DOWN_BUTTONS(5)
            if (event_val < 0) {
                this->ButtonStates[Button::BUTTON_UP]  = true;
            } else if (event_val > 0) {
                this->ButtonStates[Button::BUTTON_DOWN] = true;
            } else {
                this->ButtonStates[Button::BUTTON_UP]  = false;
                this->ButtonStates[Button::BUTTON_DOWN] = false;
            }
        }
    }
}

bool Joystick::get_button_state(Button button) {
    LOCK_THIS_SCOPE
    return this->ButtonStates[button];
}

float Joystick::get_axis_value(Axis axis) {
    LOCK_THIS_SCOPE
    short int axis_val = this->AxesValues[axis];
    float normalized_axis_val,x3_applied_axis_val;
    if (axis == Axis::SLIDER) {
        normalized_axis_val = static_cast<float>(axis_val+32768) / 65535.0f;
        return normalized_axis_val;
    } else {
        normalized_axis_val = (axis_val > 0) ? (static_cast<float>(axis_val) / 32767.0f) : (static_cast<float>(axis_val) / 32768.0f);
    }
    x3_applied_axis_val = normalized_axis_val*normalized_axis_val*normalized_axis_val;
    return x3_applied_axis_val;
}