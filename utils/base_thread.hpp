#ifndef __UTILS_BASE_THREAD_HPP__
#define __UTILS_BASE_THREAD_HPP__
#include <thread>
#include <memory>
#include "macro.hpp"
#include "noncopyable.hpp"

NS_UTILS_BEGIN
class BaseThread : public Noncopyable
{
public:
    template <typename Fuc>
    BaseThread(Fuc f) : m_thread(f){}
    ~BaseThread() {
        if(m_thread.joinable()){
            m_thread.join();
        }
    }

    void join() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

private:
    std::thread m_thread;
};
typedef std::shared_ptr<BaseThread> BaseThreadPtr;
NS_UTILS_END

#endif
