#include "scheduler.hpp"

NS_UTILS_BEGIN

Scheduler::Scheduler(uint32 mode) {
    m_mode = mode;
    m_start = false;
    if (m_mode == SCHEDULER_MODE_PRODUCER) {
        m_reactorStart.store(true);
        m_workStart.store(false);
    } else if (m_mode == SCHEDULER_MODE_CONSUMER) {
        m_reactorStart.store(false);
        m_workStart.store(true);
    } else {
        m_reactorStart.store(true);
        m_workStart.store(true);
    }
}

void Scheduler::start() {
    m_start = true;
    //all work
    m_startCond.notify_all();
}

void Scheduler::stop() {
    m_reactorStart.store(false);
    m_workStart.store(false);
    m_workList.notify(true);
}

void Scheduler::post(UnitPtr &ptr) {
    if (ptr == NULL) {
        return;
    }
    m_workList.pushWithNotify(ptr);
}

void Scheduler::workThreadFunc() {
    startWait();

    do {
        consumerFunc();
    } while(m_workStart);
}

void Scheduler::reactorThreadFunc() {
    startWait();

    do {
        producerFunc();
        timerFunc();
    } while(m_reactorStart);
}

void Scheduler::producerFunc() {
    //TODO
}

void Scheduler::timerFunc() {
    //TODO
}

void Scheduler::consumerFunc() {
    UnitPtr pUnit = m_workList->getTast();
    if (pUnit) {
        pUnit->complete();
    } else if (m_workStart) {
        m_workList.wait();
    }
}

void Scheduler::startWait() {
    std::unique_lock<std::mutex> lock(m_startMutex);
    m_startCond.wait(lock,[this](){ return this->m_start; });
}

NS_UTILS_END
