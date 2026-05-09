#ifndef JOYSTICK
#define JOYSTICK
#include <linux/joystick.h>

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

enum Axes {
    ROLL         = 0,
    PITCH        = 1,
    YAW          = 2,
    THROTTLE     = 3,
};

class Joystick {
    private:
        /**
         * This struct necessary to read data by `<linux/joystick.h>`
         */
        struct js_event event;
    
        /**
         * An array that holds buttons states on boolean array with 16 elements which
         * presents buttons on Logitech Extreme Pro 3D joystick respectively.
         */
        bool* ButtonArray;

        /**
         * An array that holds raw axes values on short int array with 
         * 4 elements presenting Roll, Pitch, Yaw, Throttle respetively.
         */
        short int* AxesValues; 

        /**
         * Used to get button state given in number parameter.
         */
        bool _get_button_state(int number);

        /**
         * Used to get axes values with given number parameter 
         * (The nunmbers 0, 1, 2, and 3 represent Roll, Pitch, Yaw, and Throttle, respectively)
         */
        short int get_axes_value(int number);

    public:
        /**
         * `Joystick` Class constructur method.
         * This method initialize all members.
         */
        Joystick(const char* device_path);
        /**
         * Used to get button state.
         * @param button Button enum field.
         * @returns Given button's state as boolean.
         */
        bool get_button_state(Button button);

        /**
         * Used to get axes value normalized [1,-1]
         * @param axes Axes enum filed.
         * @returns Given axes' value normalized [1, -1].
         */
        float get_axes_state(Axes axes);
};
#endif // JOYSTICK