#include "scheduler_safe_unit.hpp"

NS_UTILS_BEGIN

SchedulerSafeUnit::SchedulerSafeUnit(SchedulerBase& base)
: m_scheduler(base) {
    m_unitPtr = std::shared_ptr<SchedulerUnit>();
    m_unitPtr->func = std::bind(&SchedulerSafeUnit::complete, this);
    m_unitPtr->status.store(STATUS_EMPTY);
}

void SchedulerSafeUnit::post(UnitFunc func) {
    if (STATUS_EMPTY == m_unitPtr->status.load()) {
        m_unitPtr->accessList.push_back(func);
        m_unitPtr->status.store(STATUS_RUN);
        UnitPtr pUnit = m_unitPtr;
        //TODO 这里是传到哪里了 实现呢?
        m_scheduler.post(pUnit);
    } else if (STATUS_RUN == m_unitPtr->status.load()) {
        std::unique_lock<std::mutex> lock(m_unitPtr->waitMutex);
        m_unitPtr->waitList.push_back(func);
    }
}

void SchedulerSafeUnit::complete() {
    std::list<UnitFunc> runList;
    m_unitPtr->accessList.swap(runList);

    for (auto& iter : runList) {
        iter();
    }
    {
        std::unique_lock<std::mutex> lock(m_unitPtr->waitMutex);
        m_unitPtr->waitList.swap(m_unitPtr->accessList);
    }
    if (m_unitPtr->waitList.size() == 0) {
        m_unitPtr->status.store(STATUS_EMPTY);
    } else {
        UnitPtr pUnit = m_unitPtr;
        m_scheduler.post(pUnit);
    }
}


NS_UTILS_END
