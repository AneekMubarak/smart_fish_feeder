#include "../includes/SevenSegmentDriver.hpp"
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>


#define ON 1
#define OFF 0



SevenSegmentDriver::SevenSegmentDriver (gpiod_chip* chip_name, std::array <int,12> gpio_line_nums)
    :chip(chip_name){

        // Setup the 4 digit select pins as output in the digit_select_lines array
        for (int i{0}; i<4; i++){
            digit_select_lines.at(i) = gpiod_chip_get_line(chip,gpio_line_nums.at(i));


            if(!digit_select_lines.at(i)){
                throw std::runtime_error("Failed to get line: " +std::to_string(gpio_line_nums.at(i)));
            }

            if(gpiod_line_request_output(digit_select_lines.at(i), "fish_feeder", 0) < 0){
                throw std::runtime_error("Failed to set line as output: " +std::to_string(gpio_line_nums.at(i)));
            }

        }


        // Setup the 8  pins to control the segments and the dp as outputs in the led_segment_lines array
        for (int i{4}; i<12; i++){
            led_segment_lines.at(i-4) = gpiod_chip_get_line(chip,gpio_line_nums.at(i));


            if(!led_segment_lines.at(i-4)){
                throw std::runtime_error("Failed to get line: " +std::to_string(gpio_line_nums.at(i)));
            }

            if(gpiod_line_request_output(led_segment_lines.at(i-4), "fish_feeder", 0) < 0){
                throw std::runtime_error("Failed to set line as output: " +std::to_string(gpio_line_nums.at(i)));
            }

        }

    }


void SevenSegmentDriver::displayTime (Interval time_to_disp){

    m_time_to_disp = time_to_disp;
    
    _get_digits(); // seperate the time into 4 individual integers for displaying on the 7seg
    
    std::array<int,8> num_code; // to store the binary represantation of the current digit being lit

    //loop through the digit_select pins
    for (int i{0}; i<4;i++){
        // turn the current pin ON
        gpiod_line_set_value(digit_select_lines.at(i),ON);

        // get the binary code into num_code based on the current digit being lit
        if (i==0){
            num_code = _get_number_code(hour_1);
        }else if (i==1){
            num_code = _get_number_code(hour_2);
            num_code.at(0) = OFF ; // add the decimal point after the second digit to get HH.YY format (active low)
        }else if (i==2){
            num_code = _get_number_code(min_1);
        }else if (i==3){
            num_code = _get_number_code(min_2);
        }


        // light the segments
        for (int j{0}; j < 8; ++j){
            gpiod_line_set_value(led_segment_lines.at(j),num_code.at(j));

        }

        // add 1ms delay to make the segments visible when switching (lower the refresh rate)
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
        gpiod_line_set_value(digit_select_lines.at(i),OFF); // turn the current digit select line off

    }

}

void SevenSegmentDriver::release_pins(){
    // release lines
    
    for (gpiod_line* &line : digit_select_lines){
        if(line){
            gpiod_line_release(line);
        }
    }

    for (gpiod_line* &line : led_segment_lines){
        if(line){
            gpiod_line_release(line);
        }    
    }


}


// seperate the hour and minute into individual digits
void SevenSegmentDriver::_get_digits(){

    hour = m_time_to_disp.get_interval_hour();
    minute = m_time_to_disp.get_interval_minute();

    hour_1 = hour/10;
    hour_2 = hour%10;

    min_1 = minute/10;
    min_2 = minute%10;

}


// return active low binary values for displaying digits 0 to 9
std::array<int,8> SevenSegmentDriver::_get_number_code(int num){

    switch (num){
        case 0: return{1,1,0,0,0,0,0,0};
        case 1: return {1,1,1,1,1,0,0,1};
        case 2: return {1,0,1,0,0,1,0,0};
        case 3: return {1,0,1,1,0,0,0,0};
        case 4: return {1,0,0,1,1,0,0,1};
        case 5: return {1,0,0,1,0,0,1,0};
        case 6: return {1,0,0,0,0,0,1,0};
        case 7: return {1,1,1,1,1,0,0,0};
        case 8: return {1,0,0,0,0,0,0,0};
        case 9: return {1,0,0,1,0,0,0,0};
        default:
            throw std::invalid_argument("Incorrect value for displating on 7 seg display: " + std::to_string(num));


    }
}




