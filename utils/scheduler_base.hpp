#ifndef __UTILS_SCHEDULER_BASE_HPP__
#define __UTILS_SCHEDULER_BASE_HPP__

#include <functional>
#include <list>
#include <memory>

#include "macro.hpp"

NS_UTILS_BEGIN

// unit func
typedef std::function<void(void)> UnitFunc;

// unit base
class UnitBase {
public:
    virtual void complete() = 0;
};

typedef std::shared_ptr<UnitBase> UnitPtr;


// scheduler base
class SchedulerBase {
public:
    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void post(UnitPtr& pUnit) = 0;
};

//reactor base
class ReactorBase {
public:
    virtual void reactorWait(std::list<UnitPtr>& taskList) = 0;
};

typedef std::shared_ptr<ReactorBase> ReactorPtr;

NS_UTILS_END

#endif
