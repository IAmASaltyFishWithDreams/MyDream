#ifndef __UTILS_REACTOR_HPP__
#define __UTILS_REACTOR_HPP__

#include "scheduler_base.hpp"

NS_UTILS_BEGIN

class FakeReactor : public ReactorBase {
public:
    void reactorWait(std::list<UnitPtr>& taskList);
};

NS_UTILS_END
#endif
