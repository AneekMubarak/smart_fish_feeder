#include <chrono>
#include "Interval.hpp"


// allows to set a time interval for feeding in the format of hours:minutes

class FeederScheduler{
	
private:

	//MEMBERS
	// the hour and the minute for the interval
	Interval interval{};
	
	std::chrono::system_clock::time_point last_feed_time;
	std::chrono::system_clock::time_point next_feed_time;
	
	//FUNCTIONS
	std::chrono::system_clock::time_point get_next_feeding_time() const;
	
	int getHour(const std::chrono::system_clock::time_point& tp) const;
	int getMinute(const std::chrono::system_clock::time_point& tp) const;
	int getSecond(const std::chrono::system_clock::time_point& tp) const;
	
	int verify_hour(int hour) const;
	int verify_minute(int minute) const;


	
public:
	
	FeederScheduler();
	FeederScheduler(int hour, int minute);
	
	
	
	void update_feed_interval(int hour,int minute);
	void update_feed_interval (int hour);
	
	int get_interval_hour() const;
	int get_interval_minute() const;
	
	int get_next_feeding_time_hour() const;
	int get_next_feeding_time_minute() const;
	int get_next_feeding_time_second() const;
		
	// true if interval has approached
	int interval_elapsed();
	
	
	


// set interval

// if interval has approached



	
	
	
};
