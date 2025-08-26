#include "../includes/FeedDispenser.hpp"
#include <iostream>
#include <string>
#include <string_view>


#include <gpiod.h>
// #include <thread>
// #include <chrono>

int main(){

    const char* chipname = "gpiochip4";
    const unsigned int servo_line_num = 22;

    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if(!chip){
        std::cerr<<"Failed to open chip\n";
        return 1;
    }

    gpiod_line* servoLine = gpiod_chip_get_line(chip,servo_line_num);

    if(!servoLine){
        std::cerr << "Failed to get line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(servoLine, "servo", 0) < 0) {
        std::cerr << "Failed to request line 1 as output\n";
        gpiod_chip_close(chip);
        return 1;
    }

    std::cout<<"Aneek\n";

    FeedDispenser d1 {servoLine}
    d1.dispense();





    gpiod_line_release(servoLine);

    gpiod_chip_close(chip);
    return 0;





    
}