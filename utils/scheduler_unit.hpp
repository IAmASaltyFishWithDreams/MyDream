#ifndef __UTILS_SCHEDULER_UNIT_HPP__
#define __UTILS_SCHEDULER_UNIT_HPP__
#include <atomic>
#include <functional>
#include <list>
#include <mutex>

#include "noncopyable.hpp"
#include "scheduler_base.hpp"

NS_UTILS_BEGIN
class SchedulerUnit : public Noncopyable,UnitBase {
public:
    SchedulerUnit(UnitFunc func): m_func(func) {}
    void complete() { m_func(); };
private:
    UnitFunc m_func;
};

typedef std::shared_ptr<SchedulerUnit> SchedulerUnitPtr;
NS_UTILS_END

#endif
