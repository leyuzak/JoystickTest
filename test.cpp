#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

// #define JS_EVENT_BUTTON     0x01
// #define JS_EVENT_AXIS       0X02
// #define JS_EVENT_INIT       0x80


int main() {
    // const char* deviceRoute = "/dev/input/js0";
    int js = open("/dev/input/js0", O_RDONLY);
    if (!js) {
        std::cerr<<"Hatalı dosya!\n"<<std::endl;
    }

    while(true) {
        struct js_event event;
        read(js, &event, sizeof(event));
        if (event.type == JS_EVENT_AXIS || event.type == JS_EVENT_BUTTON) {
            std::cout << "time: " << event.time << "\t\t"
                      << " value: " << event.value << "\t\t"
                      << " type: " << (int)event.type << "\t\t"
                      << " number: " << (int)event.number << std::endl;
        }
    }
    

}