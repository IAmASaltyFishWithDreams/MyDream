#ifndef __UTILS_SCHEDULER_LIST_HPP__
#define __UTILS_SCHEDULER_LIST_HPP__

#include <condition_variable>
#include <map>
#include <mutex>

#include "macro.hpp"
#include "scheduler_base.hpp"

NS_UTILS_BEGIN

class SchedulerList {
public:
    // wait data
    void wait();
    //start work true all / false one
    void notify(bool all);
    //add task and acitve 
    void pushWithNotify(UnitPtr& ptr);
    //add task but not acitve
    void pushWithoutNotify(UnitPtr& ptr);
    //add batch task and active
    void pushBatchWithNotify(std::list<UnitPtr>& taskList);
    //add batch task but not active
    void pushBatchWithoutNotify(std::list<UnitPtr>& taskList);
    //get one task
    UnitPtr getTask();
    //get all task
    void getTaskList(std::list<UnitPtr>& list);
    //get task size
    uint32 getTaskSize() { return m_taskList.size(); }

private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::list<UnitPtr> m_taskList;
};

typedef std::shared_ptr<SchedulerList> SchedulerListPtr;

NS_UTILS_END

#endif
