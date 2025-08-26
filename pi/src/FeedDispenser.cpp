
# include "../includes/FeedDispenser.hpp"

#include <thread>
#include <chrono>
#include <iostream>


#define ON 1
#define OFF 0


FeedDispenser::FeedDispenser(gpiod_line* line, int start_position) // second para is optional
    :m_line(line),m_pulse_duration_us(start_position){}

FeedDispenser::FeedDispenser(gpiod_line* line)
    :FeedDispenser(line,1000){}



void FeedDispenser::move_servo(){ // cretaes a pulse on the output pin for the desired pulse length
    gpiod_line_set_value(m_line,ON);
    std::this_thread::sleep_for(std::chrono::microseconds(m_pulse_duration_us));
    gpiod_line_set_value(m_line,OFF);
    std::this_thread::sleep_for(std::chrono::microseconds(PULSE_PERIOD_US - m_pulse_duration_us));
}

void FeedDispenser::dispense(){

    for(;;){
        if(m_pulse_duration_us >= 2500){
            move_direction = 0; //go reverse
        }

        move_servo();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        move_direction ? m_pulse_duration_us+=100 : m_pulse_duration_us-=100;

        if(m_pulse_duration_us < 1000){
            break;
        }


    }
}







