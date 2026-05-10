#include <iostream>
#include "joystick.hpp"

void button_test(Joystick* js) {
    while(true){
        std::cout<<"Btn1: " << js->get_button_state(BUTTON_1)
                <<"\tBtn2: " << js->get_button_state(BUTTON_2)
                <<"\tBtn3: " << js->get_button_state(BUTTON_3)
                <<"\tBtn4: " << js->get_button_state(BUTTON_4)
                <<"\tBtn5: " << js->get_button_state(BUTTON_5)
                <<"\tBtn6: " << js->get_button_state(BUTTON_6)
                <<"\tBtn7: " << js->get_button_state(BUTTON_7)
                <<"\tBtn8: " << js->get_button_state(BUTTON_8)
                <<"\tBtn9: " << js->get_button_state(BUTTON_9)
                <<"\tBtn10: " << js->get_button_state(BUTTON_10)
                <<"\tBtn11: " << js->get_button_state(BUTTON_11)
                <<"\tBtn12: " << js->get_button_state(BUTTON_12)
                <<"\tBtnLeft: " << js->get_button_state(BUTTON_LEFT)
                <<"\tBtnRight: " << js->get_button_state(BUTTON_RIGHT)
                <<"\tBtnUp: " << js->get_button_state(BUTTON_UP)
                <<"\tBtnDown: " << js->get_button_state(BUTTON_DOWN)
                <<std::endl;
    }
}

void axes_test(Joystick* js) {
    while(true){
        printf("YAW:%05f\t\t, PITCH:%05f\t\t, ROLL:%05f\t\t, THROTTLE:%05f\n",
            js->get_axes_value(YAW), js->get_axes_value(PITCH), js->get_axes_value(ROLL), js->get_axes_value(THROTTLE));
    }
}

int main() {
    Joystick* js = new Joystick("/dev/input/js0");
    axes_test(js);
}