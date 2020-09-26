#include "time_manager.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

#include "log.hpp"
#include "time_base.hpp"
#include "time_helper.hpp"

NS_UTILS_BEGIN
static std::multimap<uint32, TimerPtr> m_timerMap;
std::shared_ptr<boost::asio::deadline_timer> TimeManager::m_timer = NULL;

bool TimeBase::afterFunc(uint32 seconds, FuncType fnc) {
    if (fnc == NULL) {
        log_error("FuncType == NULL, time id [%ul] ", m_timeId);
        return true;
    }
    m_start = TimeHelper::getCurrentSecond() + seconds;
    auto self(shared_from_this());
    return TimeManager::registrate(self);
}

void TimeBase::doComplete() {
    if (m_over) {
        return;
    }
    func();
    m_start = 0;
}

void TimeManager::init(boost::asio::io_service ios) {
    m_timer.rest(new boost::asio::deadline_timer(ios));
    m_timer->expires_from_now(TIME_MANAGER_DURING);
    m_timer->async_wait(std::bind(&TimeManager::onTimer,boost::asio::placeholders::error));
}

void TimeManager::cancel() {
    if (m_timer != NULL) {
        m_timerMap.clear();
        m_timer.cancel();
        m_timer.clear();
    }
}

bool TimeManager::registrate(TimerPtr &pTime) {
    if (pTime == NULL) {
        log_error("pTime == NULL");
        return false;
    }
    m_timerMap.insert(std::make_pair(pTime->getRunTime(), pTime));
    return true;
}

void TimeManager::onTimer(const boost::system::error_code & err) {
    if (err == boost::asio::error::operation_aborted) {
        return;
    } else if (err) {
        log_error("Time err , %s", err.message().c_str());
        return;
    }

    int32 now = TimeHelper::getCurrentSecond();
    std::vector<TimerPtr> waitingRunVec;
    for(auto iter = m_timerMap.begin(); iter != m_timerMap.end()) {
        //notes Judgment exit
        if (iter->second == NULL) {
            iter++;
            break;
        }

        if (iter->first > (uint32)now) {
            break;
        }

        waitingRunVec.push_back(iter.second);
        iter = m_timerMap.erase(iter.second);
    }
    for(TimerPtr titer : waitingRunVec) {
        if (titer == NULL) {
            continue;
        }
        titer->doComplete();
    }
    if (m_timer != NULL) {
        m_timer->expires_from_now(TIME_MANAGER_DURING);
        m_timer->async_wait(&TimeManager::onTimer,boost::asio::placeholders::error)
    }

}


NS_UTILS_END
