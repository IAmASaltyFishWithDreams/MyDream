#include "scheduler_leader_follower.hpp"
#include "reactor.hpp"

NS_UTILS_BEGIN

SchedulerLF::SchedulerLF(uint32 mode) : m_mode(mode){
    m_reactor = std::make_shared<PakeReactor>();
    m_reactorUnit = std::make_shared<SchedulerUnit>([](){});
    m_work.store(true);
}

SchedulerLF::~SchedulerLF() {

}

void SchedulerLF::start() {
    m_start = true;
    m_startCond.notify_all();
    if ((m_mode & SCHEDULE_LF_MODE_REACTOR)) {
        UnitPtr pUnit = m_reactorUnit;
        m_workList.pushWithNotify(pUnit);
    }
}

void SchedulerLF::stop() {
    m_work.store(false);
    m_workList.notify(false);
}

void SchedulerLF::post(UnitPtr& pUnit) {
    if (pUnit == NULL) {
        return;
    }
    m_workList.pushWithNotify(pUnit);
}

void SchedulerLF::run() {
    startWait();
    do{
        UnitPtr pUnit = m_workList.getTask();
        if (pUnit){
            if (pUnit == m_reactorUnit) {
                reactorFunc();
                timerFunc();
            } else {
                pUnit->complete();
            }
        } else if (m_work.load()) {
            m_workList.wait();
        }
    }while(m_work.load());
}

void SchedulerLF::reactorFunc() {
    std::list<UnitPtr> taskList;
    //this use child class func
    m_reactor->reactorWait(taskList);
    //add swith task
    taskList.push_back(m_reactorUnit);

    m_workList.pushBatchWithNotify(taskList);
}

void SchedulerLF::timerFunc(){

}

void SchedulerLF::startWait(){
    std::unique_lock<std::mutex> lock(m_startMutex);
    m_startCond.wait(lock,[this](){ return this->m_start; });
}

NS_UTILS_END
