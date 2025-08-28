
#include "../includes/FeederScheduler.hpp"
#include <ctime>

// allows to set a time interval for feeding in the format of hours:minutes

FeederScheduler::FeederScheduler(int hour, int minute)
	:last_feed_time(std::chrono::system_clock::now()){
		
	interval.set_interval(hour,minute);
	
	next_feed_time = get_next_feeding_time();

}

	
// use default interval value (12 hours)
FeederScheduler::FeederScheduler()
	:last_feed_time(std::chrono::system_clock::now()){
	next_feed_time = get_next_feeding_time();
	
	}



// SETTERS
void FeederScheduler::update_feed_interval(int hour,int minute){
	interval.set_interval(hour,minute);
	next_feed_time = get_next_feeding_time();

}


void FeederScheduler::update_feed_interval (int hour){
	interval.set_interval_hour(hour);
	next_feed_time = get_next_feeding_time();

}


//GETTERS
int FeederScheduler::get_interval_hour() const{
	return interval.get_interval_hour();
}

int FeederScheduler::get_interval_minute() const{
	return interval.get_interval_minute();
}

int FeederScheduler::get_next_feeding_time_hour() const{
	return getHour(next_feed_time);
}

int FeederScheduler::get_next_feeding_time_minute() const{
	return getMinute(next_feed_time);
}

int FeederScheduler::get_next_feeding_time_second() const{
	return getSecond(next_feed_time);
}



// CHECK FOR EVENT
int FeederScheduler::interval_elapsed(){
	
	auto now =  std::chrono::system_clock::now();
	
	if (now >= next_feed_time){
		last_feed_time = now;
		next_feed_time = get_next_feeding_time();
		return 1;
	}
	
	return 0;	
}


// GET NEXT FEEDING TIME
/**
 * PRIVATE FUNCTION
 * Calculate and return the next feeding time
 * MUST be used whenever a new time is set.
 **/
std::chrono::system_clock::time_point FeederScheduler::get_next_feeding_time() const{
	
	//calculate time for next feeding
	auto feeder_interval_hour = std::chrono::hours(interval.get_interval_hour());
	auto feeder_interval_minute = std::chrono::minutes(interval.get_interval_minute());
	
	//return next feeding time
	return last_feed_time + feeder_interval_hour + feeder_interval_minute; 
	
}

#include <chrono>
#include <ctime>

int FeederScheduler::getHour(const std::chrono::system_clock::time_point& tp) const {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
    localtime_r(&t, &tm);   
    return tm.tm_hour; // 0 to 23
}

int FeederScheduler::getMinute(const std::chrono::system_clock::time_point& tp) const {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
    localtime_r(&t, &tm);
    return tm.tm_min;  // 0 to 59
}

int FeederScheduler::getSecond(const std::chrono::system_clock::time_point& tp) const {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm{};
    localtime_r(&t, &tm);
    return tm.tm_sec;  // 0 to 59
}




