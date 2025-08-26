#pragma once

#include <gpiod.h>



class FeedDispenser{

    private:
        gpiod_line* m_line;
        int m_pulse_duration_us{1000};
        int move_direction{1}; // 1 for forward, 0  for backwards
        static constexpr int PULSE_PERIOD_US = 20000; // fixed period for pulses in the 9g servo in miliseconds

        void move_servo();


    public:
        FeedDispenser(gpiod_line* line_num, int start_position); // ranges from 1000 to 2500
        FeedDispenser(gpiod_line* line);

        void dispense();
        void dispense_with_steps(int pulse_increment, int ms_between_pules);





};