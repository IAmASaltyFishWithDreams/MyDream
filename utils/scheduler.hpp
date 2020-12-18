#ifndef __UTILS_SCHEDULER_HPP__
#define __UTILS_SCHEDULER_HPP__

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include "scheduler_base.hpp"
#include "scheduler_list.hpp"
#include "scheduler_unit.hpp"

NS_UTILS_BEGIN


// 模式:SCHEDULER_MODE_PRODUCER 生产者模式
// 线程分配:reactor Thread(1) work Thread(0)
// 场景:reactor(epoll/poll/select),构建定时器
//
// 模式:SCHEDULER_MODE_CONSUMER 消费者模式
// 线程分配:reactor Thread(0) work Thread(1~n)
// 场景:使用工作线程写日志
//
// 模式:SCHEDULER_MODE_PRODUCER/SCHEDULER_MODE_CONSUMER 双模式
// 线程分配:reactor Thread(1) work Thread(1~n)
// 场景:reactor(select/poll/epoll)socker监听 工作线程操作(send, recv, close)

#define SCHEDULER_MODE_PRODUCER 1   //生产者模式
#define SCHEDULER_MODE_CONSUMER 2   //消费者模式

//调度器(任务生产,消费,分配)
//固化线程绑定:reactorThreadFunc (selet/epoll/poll) workThreadFunc (scoket/operator)
//线程数量:reactorThread(0~1) workThread(1~n)

class Scheduler : public SchedulerBase {
public:
    Scheduler(uint32 mode);
    ~Scheduler(){}
    //开始工作
    void start();
    //结束工作
    void stop();
    //提交任务
    void post(UnitPtr& ptr);
    //反应堆线程接口
    void reactorThreadFunc();
    //work线程接口
    void workThreadFunc();

private:
    //生产
    void producerFunc();
    //消费
    void consumerFunc();
    //定时器
    void timerFunc();
    //工作开始等待
    void startWait();

private:
    //工作流程
    uint32 m_mode;                           //工作模式
    bool m_start;       

    std::mutex m_startMutex;                //开始工作锁

    std::condition_variable m_startCond;    //开始工作条件变量
    //reactor数据
    std::atomic<bool> m_reactorStart;        //reactor 开始工作
    //work 数据
    std::atomic<bool> m_workStart;          //work 工作标识

    SchedulerList m_workList;               //work工作队列
};

NS_UTILS_END

#endif
