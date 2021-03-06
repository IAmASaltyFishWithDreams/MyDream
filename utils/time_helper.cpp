#include "time_helper.hpp"

#include "ctime"
#include "iomanip"
#include "sstream"

NS_UTILS_BEGIN

int32 TimeHelper::m_initTime;
steadyClock::time_point TimeHelper::m_start;

void TimeHelper::init(int32 now) {
    m_start = steadyClock::now();
    m_initTime = now == 0 ? getSystemCurrentSecond() : now;
}

int32 TimeHelper::getCurrentSecond() {
    return std::chrono::duration_cast<second>(steadyClock::now() - m_start).count() + m_initTime;
}

int32 TimeHelper::getSystemCurrentSecond() {
    return (int32)systemClock::to_time_t(systemClock::now());
}

std::string TimeHelper::getTimeStr(int32 now) {
    std::time_t t = now;
    std::stringstream os;
    os << std::put_time(std::localtime(&t),"%Y%m %d %T");
    return os.str();
}

std::string TimeHelper::getNowTimeStr() {
    return getTimeStr(getCurrentSecond());
}

NS_UTILS_END
