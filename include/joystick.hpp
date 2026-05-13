#ifndef JOYSTICK
#define JOYSTICK
#include <linux/joystick.h>
#include <mutex>
#include <atomic>
#include <thread>

#include "AIClass.hpp"

enum Button {
    BUTTON_1     = 0,
    BUTTON_2     = 1,
    BUTTON_3     = 2,
    BUTTON_4     = 3,
    BUTTON_5     = 4,
    BUTTON_6     = 5,
    BUTTON_7     = 6,
    BUTTON_8     = 7,
    BUTTON_9     = 8,
    BUTTON_10    = 9,
    BUTTON_11    = 10,
    BUTTON_12    = 11,

    BUTTON_LEFT  = 12,
    BUTTON_RIGHT = 13,
    BUTTON_UP    = 14,
    BUTTON_DOWN  = 15,
};

enum Axis {
    ROLL         = 0,
    PITCH        = 1,
    YAW          = 2,
    SLIDER       = 3,
};

class Joystick {
    private:
        /**
         * This is an instance of AI class.
         */
        AI* ai; 

        /**
         * This is a file descripter it will taken from open() and
         * it is necessary for read() methods which both are going to use joystick's data.
         */
        int js_fd;

        /**
         * This thread will listen data from joystick asynchronously by using read() and 
         * joystick::parse_event_data() in a while loop.
         */
        std::thread listening_thread;

        /**
         * This thread manages recording audio by using `AI::start_recording()` and `AI::stop_recording()`
         */
        std::thread AI_thread;

        /**
         * This struct necessary to read data by `<linux/joystick.h>`
         */
        struct js_event event;

        /**
         * This used since program both listens to joystick and make able user to read it. Both things do not run at same time. 
         * By this mutex, these process work in harmony, they do not crash each other.
         */
        std::mutex data_mutex;

        /**
         * This atomic boolean indicates whether joystick is listening now.
         * If this is false: You will be able to read data
         * If this is true: Yo will be wait to read data since prevent to race condition. 
         */
        std::atomic<bool> data_listen_flag;
    
        /**
         * An array that holds buttons states on boolean array with 16 elements which these first 12 elements
         * presents buttons on Logitech Extreme Pro 3D joystick respectively. Last 4 element are Left, Right, Up, Down.
         */
        bool* ButtonStates;

        /**
         * An array that holds raw axes values on short int array with 
         * 4 elements presenting Roll, Pitch, Yaw, Throttle respetively.
         */
        short int* AxesValues; 

        /**
         * This method gointo parse raw event data to make able to update class axes and button 
         * fields which is more undurstandable format.
         */
        void parse_event_data();

    public:
        /**
         * `Joystick` Class constructur method.
         * This method initialize all members.
         * @param devica_path: This is joysticks path.
         */
        Joystick(const char* device_path, AI* ai);

        /**
         * stops to deteached thread that using parse_event_data() used to listen joystick
         * terminate that detached thread.
         */
        ~Joystick();

        /**
         * Used to get button state.
         * @param button Button enum field.
         * @returns Given button's state as boolean.
         */
        bool get_button_state(Button button);

        /**
         * Used to get axes value normalized [1,-1]
         * @param axes Axis enum filed.
         * @returns Given axes' value normalized [1, -1].
         */
        float get_axis_value(Axis axes);
};
#endif // JOYSTICK