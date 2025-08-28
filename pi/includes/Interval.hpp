#pragma once

#include <chrono>
#include <stdexcept>



class Interval {


private:
	// the hour and the minute for the interval
	std::chrono::hours m_interval_hour;
	std::chrono::minutes  m_interval_minute;

public:

	Interval(int hours, int minutes){
		set_interval(hours,minutes);
	}
	
	Interval (int hours){
		set_interval(hours,0);
	}
	
	Interval (){
		set_interval(12,0); // default is 12 hours in case no interval is set later on.
	}
	
	
	
	void set_interval(int hour, int minute){
		if (hour < 0){
			throw std::invalid_argument("Interval::set(), hours cannot be negative");
		}
		
		if (minute < 0 || minute >= 60 ){
			throw std::invalid_argument("Interval::set(), minutes must be from range 0-60");
		}
		
		m_interval_hour = std::chrono::hours(hour);
		m_interval_minute = std::chrono::minutes(minute);
		
	}
	
	//SETTERS
	void set_interval_hour(int hour){
		set_interval(hour, m_interval_minute.count());
	}
	
	void set_interval_minute(int min){
		set_interval(m_interval_hour.count(), min);
	
	}
	
	
	//GETTERS
	int get_interval_hour() const {
		return m_interval_hour.count();
	}
	
	int get_interval_minute() const {
		return m_interval_minute.count();
	}

	
	
	
};


