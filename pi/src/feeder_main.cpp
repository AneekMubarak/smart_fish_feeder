#include "../includes/FeedDispenser.hpp"
#include <iostream>
#include <string>
#include <string_view>



#include <gpiod.h>
#include <thread>
#include <chrono>

#define ON 1
#define OFF 0

//~ void motor_driver(gpiod_line* line, int val){
        //~ gpiod_line_set_value(line,val);
        
//~ }

int main(){

    const char* chipname = "gpiochip4";
    const unsigned int servo_line_num = 22;
    const unsigned int motor_driver_en_line_num= 17;

    

    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if(!chip){
        std::cerr<<"Failed to open chip\n";
        return 1;
    }

    gpiod_line* servoLine = gpiod_chip_get_line(chip,servo_line_num);
    gpiod_line* motor_driver_en_line = gpiod_chip_get_line(chip,motor_driver_en_line_num);


    if(!servoLine){
        std::cerr << "Failed to get line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(servoLine, "servo", 0) < 0) {
        std::cerr << "Failed to request line as output\n";
        gpiod_chip_close(chip);
        return 1;
    }
    
    
    if(!motor_driver_en_line){
        std::cerr << "Failed to get line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(motor_driver_en_line, "motor_driver", 0) < 0) {
        std::cerr << "Failed to request line as output\n";
        gpiod_chip_close(chip);
        return 1;
    }
    
    

    std::cout<<"Testing\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    FeedDispenser d1 {servoLine,motor_driver_en_line};
    d1.dispense();






    
    





    gpiod_line_release(servoLine);
    gpiod_line_release(motor_driver_en_line);


    gpiod_chip_close(chip);
    return 0;





    
}
