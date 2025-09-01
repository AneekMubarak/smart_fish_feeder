#include <iostream>
#include <string>
#include <string_view>

#include <gpiod.h>
#include <thread>
#include <chrono>
#include <array>

#include "../includes/FeedDispenser.hpp"
#include "../includes/FeederScheduler.hpp"
#include "../includes/ButtonReader.hpp"
#include "../includes/SevenSegmentDriver.hpp"


#define ON 1
#define OFF 0


int main(){

    const char* chipname = "gpiochip4";
    const unsigned int servo_line_num = 22;
    const unsigned int motor_driver_en_line_num= 17;
    const unsigned int hour_sw_num = 27;
    const unsigned int minute_sw_num = 5;

    
    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if(!chip){
        std::cerr<<"Failed to open chip\n";
        return 1;
    }

    gpiod_line* servoLine = gpiod_chip_get_line(chip,servo_line_num);
    gpiod_line* motor_driver_en_line = gpiod_chip_get_line(chip,motor_driver_en_line_num);
    gpiod_line* hour_sw_line = gpiod_chip_get_line(chip,hour_sw_num);
    gpiod_line* minute_sw_line = gpiod_chip_get_line(chip,minute_sw_num);

    if(!servoLine){
        std::cerr << "Failed to get line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(servoLine, "fish_feeder", 0) < 0) {
        std::cerr << "Failed to request line as output\n";
        gpiod_chip_close(chip);
        return 1;
    }
    
    if(!motor_driver_en_line){
        std::cerr << "Failed to get line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(motor_driver_en_line, "fish_feeder", 0) < 0) {
        std::cerr << "Failed to request line as output\n";
        gpiod_chip_close(chip);
        return 1;
    }
    
    
    if(!hour_sw_line){
        std::cerr << "Failed to get line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_input(hour_sw_line, "fish_feeder") < 0) {
        std::cerr << "Failed to request line as input\n";
        gpiod_chip_close(chip);
        return 1;
    }
    
    
    if(!minute_sw_line){
        std::cerr << "Failed to get line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_input(minute_sw_line, "fish_feeder") < 0) {
        std::cerr << "Failed to request line as input1\n";
        gpiod_chip_close(chip);
        return 1;
    }

    //Array Elements map to pins:  d1 d2 d3 d4 dp g f e d c b a
    std::array <int,12> seven_seg_lines{6,13,19,26,21,20,16,12,25,24,23,18};
    

    
    FeedDispenser dispenser {servoLine,motor_driver_en_line};      
    FeederScheduler scheduler {};
    ButtonReader hour_button {hour_sw_line};
    ButtonReader minute_button {minute_sw_line};
    SevenSegmentDriver seven_seg_display{chip,seven_seg_lines}; // 4 digit seven segment display
    
            
    auto printNextFeedingTime = [&scheduler]() {
    std::cout << "Next Feeding Time is "
              << scheduler.get_next_feeding_time_hour() << ":"
              << scheduler.get_next_feeding_time_minute() << ":"
              << scheduler.get_next_feeding_time_second()
              << "\n";
    };
    
    printNextFeedingTime();
    
    //~ std::cout << "Interval: " << scheduler.get_interval_hour() <<":"<<scheduler.get_interval_minute()<<"\n";

    
    int interval_hour{};
    int interval_minute{};

    
    while(1){
        
        interval_hour = scheduler.get_interval_hour();
        interval_minute = scheduler.get_interval_minute();
        
        if(minute_button.isPressed() && hour_button.isPressed()){ // press both buttons together to show the next feeding time instead of the interval
            // TO BE IMPLEMENTED
            std::cout << "both\n";
            
            //prevent interval from incrementing in the case of a double press 
            --interval_hour;
            --interval_minute;
        }
        
        
        // Handle pressing the increment hour button
        if(hour_button.is_valid_press()){
            scheduler.update_feed_interval(++interval_hour,interval_minute);                

        }
        
        // Handle pressing the increment minute button
        if(minute_button.is_valid_press()){
            scheduler.update_feed_interval(interval_hour,++interval_minute);

        }
        
        
        std::cout << "Interval: " << scheduler.get_interval_hour() <<":"<<scheduler.get_interval_minute()<<"\n";

        
        //dispense feed if the interval has elapsed
        if (scheduler.interval_elapsed()){
            dispenser.dispense();
            printNextFeedingTime();
        }

        // display the currently set feeding interval on the 4 digit 7 segment display in the format HH.MM
        seven_seg_display.displayTime(scheduler.get_interval());
    
    }



    seven_seg_display.release_pins();
    gpiod_line_release(servoLine);
    gpiod_line_release(motor_driver_en_line);
    gpiod_chip_close(chip);
    return 0;
    
} 




