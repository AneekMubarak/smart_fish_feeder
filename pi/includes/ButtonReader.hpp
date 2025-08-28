#pragma once
#include <gpiod.h>
#include <chrono>
#include <thread>




class ButtonReader{
	

private:
	gpiod_line* m_line;
	int prev_value;



public:
	ButtonReader(gpiod_line* line)
	:m_line(line){
	prev_value = 0;
	}
	
	
	int isPressed(){
		// button is active low
		return (gpiod_line_get_value(m_line)==0);
	}
	
	
	//DEBOUNCE THE BUTTON
	int is_valid_press(){
		int currValue = isPressed();
		if (currValue && (prev_value == 0)){
			prev_value = currValue;
			
			return 1;
		}
		
		prev_value = currValue;
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // prevent jitter

		return 0;
	}
	
	
	
	
	
	
    
	
	
};
