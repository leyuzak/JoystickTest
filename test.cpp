#include <iostream>
#include "joystick.hpp"

int main() {
    Joystick* js = new Joystick("/dev/input/js0");
    
    while(true){
        // js->get_button_state usage
        bool b_1    = js->get_button_state(BUTTON_1);
        bool b_2    = js->get_button_state(BUTTON_2);
        bool b_3    = js->get_button_state(BUTTON_3);
        bool b_4    = js->get_button_state(BUTTON_4);
        bool b_5    = js->get_button_state(BUTTON_5);
        bool b_6    = js->get_button_state(BUTTON_6);
        bool b_7    = js->get_button_state(BUTTON_7);
        bool b_8    = js->get_button_state(BUTTON_8);
        bool b_9    = js->get_button_state(BUTTON_9);
        bool b_10   = js->get_button_state(BUTTON_10);
        bool b_11   = js->get_button_state(BUTTON_11);
        bool b_12   = js->get_button_state(BUTTON_12);
        bool b_L    = js->get_button_state(BUTTON_LEFT);
        bool b_R    = js->get_button_state(BUTTON_RIGHT);
        bool b_U    = js->get_button_state(BUTTON_UP);
        bool b_D    = js->get_button_state(BUTTON_DOWN);

        // js->get_axis_value usage
        float yaw       = js->get_axis_value(YAW);
        float pitch     = js->get_axis_value(PITCH);
        float roll      = js->get_axis_value(ROLL);
        float throttle  = js->get_axis_value(SLIDER);
        
        std::cout<<"Btn1: " << b_1 <<"\tBtn2: " << b_2 <<"\tBtn3: " << b_3
                 <<"\tBtn4: " << b_4 <<"\tBtn5: " << b_5 <<"\tBtn6: " << b_6
                 <<"\tBtn7: " << b_7 <<"\tBtn8: " << b_8 <<"\tBtn9: " << b_9
                 <<"\tBtn10: " << b_10 <<"\tBtn11: " << b_11 <<"\tBtn12: " << b_12
                 <<"\tBtnL: " << b_L <<"\tBtnR: " << b_R <<"\tBtnU: " << b_U <<"\tBtnD: " << b_D
                 <<"\tYaw: " << yaw <<"\tPitch: " << pitch <<"\tRoll: " << roll <<"\tSlider: " << throttle
                 <<std::endl;
    }
}