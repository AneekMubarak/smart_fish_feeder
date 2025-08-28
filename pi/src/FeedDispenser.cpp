
# include "../includes/FeedDispenser.hpp"

#include <thread>
#include <chrono>
#include <iostream>


#define ON 1
#define OFF 0




FeedDispenser::FeedDispenser(gpiod_line* line, gpiod_line* driver_enable_line,  int start_position) // second para is optional
    :m_line(line),m_pulse_duration_us(start_position),m_driver_line(driver_enable_line){}

FeedDispenser::FeedDispenser(gpiod_line* line, gpiod_line* driver_enable_line)
    :FeedDispenser(line,driver_enable_line,1000){}



// cretaes a pulse on the output pin (from 1-2.5ms) to move to the desired location.
void FeedDispenser::move_servo(){ 
    gpiod_line_set_value(m_line,ON);
    std::this_thread::sleep_for(std::chrono::microseconds(m_pulse_duration_us));
    gpiod_line_set_value(m_line,OFF);
    std::this_thread::sleep_for(std::chrono::microseconds(PULSE_PERIOD_US - m_pulse_duration_us));
}

// the servo takes pulses from 1ms to 2.5ms, this function starts at 1ms (0 deg) and moves the servo by 180 deg by gradually changing the pulse to 2.5ms and back to 0 deg
void FeedDispenser::dispense(){
    std::cout << "Dispensing...\n";
    gpiod_line_set_value(m_driver_line,ON);
    for(;;){
        if(m_pulse_duration_us >= 2500){
            move_direction = 0; //go reverse
        }

        move_servo();
        std::cout << m_pulse_duration_us << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        move_direction ? m_pulse_duration_us+=250 : m_pulse_duration_us-=250;

        if(m_pulse_duration_us < 1000){
            m_pulse_duration_us = 1000;
            move_direction = 1;
            break;        
        }
    }
    
    gpiod_line_set_value(m_driver_line,OFF);
}







