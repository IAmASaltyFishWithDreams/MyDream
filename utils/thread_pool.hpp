#ifndef __UTILS_THREADPOOL_HPP
#define __UTILS_THREADPOOL_HPP

#include <vector>

#include "base_thread.hpp"
NS_UTILS_BEGIN
class ThreadPool : public Noncopyable {
public:
    ThreadPool() {}
    ~ThreadPool() {
        reset();
    }
public:
    template<typename Fuc>
    void create_thread(Fuc f ,size_t threadNum){
        BaseThreadPtr ptr = std::make_shared<BaseThreadPtr>(f);
        m_threadPool_vec.push_back(ptr);
    }

    void reset(){
        m_threadPool_vec.clear();
    }
    void join() {
        for(auto &iter : m_threadPool_vec)
            if (iter != NULL) {
                iter->join();
            }
    }

private:
    std::vector<BaseThreadPtr> m_threadPool_vec;
};
NS_UTILS_END
#endif
