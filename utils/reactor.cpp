#include "reactor.hpp"

#include "chrono"
#include "thread"

#include "log.hpp"
#include "rand.hpp"
#include "scheduler_unit.hpp"

NS_UTILS_BEGIN

void FakeReactor::reactorWait(std::list<UnitPtr>& taskList) {
    log_debug("thread [%ul], run reactor wait ", std::this_thread::get_id());

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    uint32 count = Rand::randBetween((uint32)1, (uint32)5);
    for (uint32 i = 0; i < count; i++ ){
        UnitPtr ptr = std::make_shared<SchedulerUnit>([](){
            log_debug("随机任务 [%ul]", std::this_thread::get_id());
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        });
        taskList.push_back(ptr);
    }
}

NS_UTILS_END
