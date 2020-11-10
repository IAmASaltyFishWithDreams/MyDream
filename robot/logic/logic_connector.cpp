#include "logic_connector.hpp"

#include "../main/robot.hpp"
#include "../main/robot_global.hpp"
#include "../../utils/log.hpp"
#include "../../utils/macro.hpp"

USING_NS_BOOST_NETWORK

void LogicConnector::onNewMsg(const TcpSocketPtr& s, uint16 cmd, const MsgBufPtr& buf) {
    global::pRobot->getIos().post(std::bind(&Robot::onLogicMsg, global::pRobot, s, cmd, buf));
}

void LogicConnector::onConnect(const BOOST_NETWORK::TcpSocketPtr& s) {
    global::pRobot->getIos().post(std::bind(&Robot::onLogicConnect, global::pRobot, s));
}

void LogicConnector::onClose(const BOOST_NETWORK::TcpSocketPtr& s) {
    global::pRobot->getIos().post(std::bind(&Robot::onLogicClose, global::pRobot, s));
}
