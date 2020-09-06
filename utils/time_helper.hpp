#ifndef __UTILS_TIME_HELPER_HPP__
#define __UTILS_TIME_HELPER_HPP__
#include <string>
#include "time_base.hpp"
NS_UTILS_BEGIN

/*
 * this timer c++11
 * base time  time_point
 * */
class StopWatch {
public:
    StopWatch(): m_start(now()){}
    ~StopWatch(){}
    //reset time_point
    void reset(){
        m_start = now();
    }
public:
    //already time (us)
    int32 duratonUs() {
        return std::chrono::duration_cast<microsecond>(now()-m_start).count();
    }
    //already time (ms)
    int32 durationMs() {
        return std::chrono::duration_cast<millisecond>(now()-m_start).count();
    }
    //already time (s)
    int32 durationS() {
        return std::chrono::duration_cast<second>(now()-m_start).count();
    }

private:
    //get current time
    static steadyClock::time_point now(){
        return steadyClock::now();
    }

private:
    steadyClock::time_point m_start;
};

class TimeHelper {

public:
    static void init(int32 now = 0);

    static int32 getCurrentSecond();

    static int32 getSystemCurrentSecond();

    static std::string getTimeStr(int32 now);

    static std::string getNowTimeStr();

private:
    static int32 m_initTime;
    static steadyClock::time_point m_start;
};

NS_UTILS_END
#endif
