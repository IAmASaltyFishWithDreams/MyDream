#include "robot_global.hpp"
#include "robot.hpp"
#include "../../utils/log.hpp"

Robot* global::pRobot = NULL;

int main() {
    Robot  robot;
    global::pRobot = &robot;
    if (!pRobot->runServer()) {
        log_trac("***********")
        return 0;
    }
}
