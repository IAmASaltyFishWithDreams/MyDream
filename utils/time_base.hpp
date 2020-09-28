#ifndef __UTILS_TIME_BASE_HPP__
#define __UTILS_TIME_BASE_HPP__
#include <chrono>
#include "macro.hpp"
NS_UTILS_BEGIN

//system time
typedef std::chrono::system_clock systemClock;
//a steady time
typedef std::chrono::steady_clock steadyClock;

//hours std::ratio<3600:1>  ---<s,hours>
typedef std::chrono::hours hour;
//minutes std::ratio<60:1> ---<s,minutes>
typedef std::chrono::minutes minute;
//seconds std::ratio<1:1> ---<s,s>
typedef std::chrono::seconds second;
//milliseconds std::ratio<1:1000> ---<s,millisec>
typedef std::chrono::milliseconds millisecond;
//microseconds std::ratio<1:1000000> ---<s,micorsec>
typedef std::chrono::microseconds microsecond;

#define TIME_MANAGER_DURING boost::posix_time::millisec(1000);

NS_UTILS_END

#endif
