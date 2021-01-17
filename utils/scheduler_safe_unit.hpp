#ifndef __UTILS_SCHEDULER_SAFE_UNIT_HPP__
#define __UTILS_SCHEDULER_SAFE_UNIT_HPP__

#include <atomic>
#include <functional>
#include <list>
#include <mutex>

#include "noncopyable.hpp"
#include "scheduler_base.hpp"
#include "log.hpp"

NS_UTILS_BEGIN
//禁止 copy and move
class SchedulerSafeUnit : public Noncopyable {
enum {
    STATUS_EMPTY = 0,
    STATUS_RUN = 1,
};

class SafeUnit : public UnitBase, Noncopyable {
friend SchedulerSafeUnit;
private:
    void complete() { func(); }

private:
    std::atomic<int> status;            //status
    std::mutex waitMutex;               //wait mutex
    std::list<UnitFunc> waitList;       //wait list
    std::list<UnitFunc> accessList;     //run  list
    UnitFunc func;                      //run func
};

public:
    SchedulerSafeUnit(SchedulerBase& base);
    ~SchedulerSafeUnit(){}
    //delete construct func
    SchedulerSafeUnit(SchedulerSafeUnit && unit) = delete;
    void complete();

    void post(UnitFunc func);

    SchedulerBase& m_scheduler;
    std::shared_ptr<SafeUnit> m_unitPtr;
};

typedef std::shared_ptr<SchedulerSafeUnit> SchedulerSafeUnitPtr;

NS_UTILS_END

#endif
