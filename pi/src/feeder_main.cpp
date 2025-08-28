#include <iostream>
#include <string>
#include <string_view>

#include "../includes/FeedDispenser.hpp"
#include "../includes/FeederScheduler.hpp"

    

#include <gpiod.h>
#include <thread>
#include <chrono>

#define ON 1
#define OFF 0

//~ std::chrono::seconds feed_interval_sec{15};
std::chrono::system_clock::time_point last_feed_time = std::chrono::system_clock::now();

int checkSchedule(std::chrono::seconds sec){

    
    
    auto now = std::chrono::system_clock::now();
    
    auto next_feed_time = last_feed_time + sec;
    
    if(now >= next_feed_time){
        last_feed_time = now;
        return 1;
    }
    
    return 0;
}

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
    

    

    std::cout<<"Testing\n";
    
    FeedDispenser d1 {servoLine,motor_driver_en_line};
    FeederScheduler scheduler {0,1};
            //~ std::cout << "Next Feeding Time is " <<scheduler.get_next_feeding_time_hour() 
            //~ << ":" <<scheduler.get_next_feeding_time_minute() <<":" <<scheduler.get_next_feeding_time_second() << "\n";
            
    auto printNextFeedingTime = [&scheduler]() {
    std::cout << "Next Feeding Time is "
              << scheduler.get_next_feeding_time_hour() << ":"
              << scheduler.get_next_feeding_time_minute() << ":"
              << scheduler.get_next_feeding_time_second()
              << "\n";
    };
    
    printNextFeedingTime();

    
    while(1){
        
        
        
        if (scheduler.interval_elapsed()){
            d1.dispense();
            printNextFeedingTime();
        }
        
        
    }




    gpiod_line_release(servoLine);
    gpiod_line_release(motor_driver_en_line);


    gpiod_chip_close(chip);
    return 0;





    
}
