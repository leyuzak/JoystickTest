#include "joystick.h"
#include <iostream>

int main()
{
    Joystick js;
    js.start();

    while (true)
    {
        auto d = js.getLatestData();

        if (d.isPressed(Button::Trigger)) {
            std::cout << "TRIGGER\n";
        }
    }

    return 0;
}