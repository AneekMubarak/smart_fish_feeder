#pragma once

#include <array>
#include <gpiod.h>
#include "Interval.hpp"

class SevenSegmentDriver {


private:
    Interval m_time_to_disp{};
    gpiod_chip* chip;

    // the hour and the minute being displayed
    int hour{};
    int minute{};

    // the individual digits of the hour and minute
    int hour_1{}; // tenth position of hour
    int hour_2{}; //ones position of hour
    int min_1{}; // tenth position of minute
    int min_2{}; //ones position of minute
    
    //Array Elements map to pins:  d1 d2 d3 d4 (Digit Select Pins)
    std::array<gpiod_line*,4> digit_select_lines {};


    //Array Elements map to pins: dp g f e d c b a in order  (Segment data pins)
    std::array<gpiod_line*,8> led_segment_lines {};

    void _get_digits(); // seperate the houn and minute into their individual digits

    std::array<int,8> _get_number_code(int); // the avtive low binary code in an array for a digit
    


public:
    SevenSegmentDriver (gpiod_chip* chip_name, std::array <int,12> gpio_line_nums); // array of the form d1 d2 d3 d4 dp g f e d c b a

    void displayTime (Interval time_to_disp); // display the time on the 4 digit seven segment display
    void release_pins();




};