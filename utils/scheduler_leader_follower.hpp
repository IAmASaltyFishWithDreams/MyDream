#ifndef __UTILS_SCHEDULER_LEADER_FOLLOWER_HPP__
#define __UTILS_SCHEDULER_LEADER_FOLLOWER_HPP__

#include "atomic"
#include "condition_variable"
#include "map"
#include "mutex"
#include "thread"

#include "scheduler_base.hpp"
#include "scheduler_list.hpp"
#include "scheduler_unit.hpp"

NS_UTILS_BEGIN

//reactor mode
#define SCHEDULE_LF_MODE_REACTOR 0x01
//task list
#define SCHEDULE_LF_MODE_TASK_LIST 0x02

//reactor && task list
class SchedulerLF : SchedulerBase {
public:
    SchedulerLF(uint32 mode);
    ~SchedulerLF();

    void start();

    void stop();

    void post(UnitPtr &ptr);

    void run();

private:
    void reactorFunc();

    void timerFunc();

    void otherFunc();

    void startWait();

private:
    uint32 m_mode;
    bool m_start;
    std::mutex m_startMutex;
    std::condition_variable m_startCond;

    std::atomic<bool> m_work;
    SchedulerList m_workList;

    ReactorPtr m_reactor;
    SchedulerUnitPtr m_reactorUnit;

};

NS_UTILS_END

#endif
