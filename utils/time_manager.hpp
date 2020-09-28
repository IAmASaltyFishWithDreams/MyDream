#ifndef __UTILS_TIME_MANAGER_HPP__
#define __UTILS_TIME_MANAGER_HPP__
#include <functional>
#include <map>

#include "boost/asio/deadline_timer.h"
#include "boost/asio/io_service.h"
#include "boost/system/error_code.h"

#include "macro.hpp"

NS_UTILS_BEGIN

class TimeBase : public std::enable_shared_from_this<TimeBase> {
    enum TimerState {
        TS_CLOST = 0,
        TS_START = 1,
    };
    typedef std::function<void()> FuncType;

public:
    TimeBase(uint64 timeId) : m_over(false),m_timeId(timeId){};
    ~TimeBase(){};

public:
    void cancel() { m_over = true; }
    bool isOver() { return m_over; }
    bool afterFunc(uint32_t seconds, FuncType func);
    void doComplete();
    uint64 getTimerId() { return m_timeId; }
    uint32 getRunTime() { return m_start; }

private:
    bool m_over;        //over
    uint64 m_timeId;    //time id
    FuncType func;      // exe
    uint32 m_start;     // start
    TimerState m_state = TimerState::TS_CLOST; //default close
};

typedef  std::shared_ptr<TimeBase> TimerPtr;

class TimeManager {
public:
    TimeManager() {}
    ~TimeManager() {}
public:
    static void init(boost::asio::io_service& ios);
    static void cancel();
    static bool registrate(TimerPtr& pTime);
    static void onTimer(const boost::system::error_code& err);
private:
    static std::multimap<uint32, TimerPtr> m_timerMap;
private:
    static std::shared_ptr<boost::asio::deadline_timer> m_timer;
};
NS_UTILS_END
#endif
