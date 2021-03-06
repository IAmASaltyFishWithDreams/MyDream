#include "robot.hpp"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property/xml_parser.hpp>

#include "../logic/logic_msg_handler.hpp"
#include "../../utils/time_helper.hpp"
#include "../../utils/log.hpp"

#include "protocpp/op_cmd.hpp"
#include "protocpp/protos/server.bp.h"

USING_NS_ERVICE_CORE
USING_NS_UTILS
USING_NS_PROTOCPP

bool Robot::onInitServer() {
    if(!onLoadConfig()) {
        log_error("load config failed.");
        return false;
    }
    if(!onInitNetWork()) {
        log_error("init network failed.");
        return false;
    }
    if(!onInitLogic()) {
        log_error("init logic failed.");
        return false;
    }
    return true;
}

void Robot::onRunServer() {
    log_infos("[%s start at %s]", getServerName(), TimeHelper::getNowTimeStr().c_str());
    setbuf(stdout, NULL);
}

bool Robot::onCloseServer() {
    closeNetwork();
    return true;
}

bool Robot::onLoadConfig() {
    if (!onLoadXmlConfig()) {
        return false;
    }
    return true;
}

bool Robot::onInitNetWork() {
    //notes fix config
    startNetwork(1);

    connectTo("111.220.231.150",23333, &m_connector, 10);

    LogicMsgHandler::initMsgHandler();
    return true;
}

bool Robot::onInitLogic() {
    CmdManager::init();
    return true;
}

bool Robot::onLoadXmlConfig() {
    return true;
}

void Robot::onLogicConnect(const BOOST_NETWORK::TcpSocketPtr& s) {
    proto::server::MsgRobot msg;
    msg.set_test("robot come on ");
    sendMsgBySocket(s,MsgRobot,msg);

    //proto::server::MsgTestTcpRequest request;
    //request.set_count(6)
    //sendMsgBySocket(s, MsgTestTcpRequest, request);
}

void Robot::onLogicMsg(const BOOST_NETWORK::TcpSocketPtr& s, uint16 cmd, const BOOST_NETWORK::MsgBufPtr& buf) {
    LogicMsgHandler.s_msgHandler.msgHandler(s,cmd,buf,m_connector.name());
}

void Robot::onLogicClose(const BOOST_NETWORK::TcpSocketPtr& s) {
    closeNetwork();
}
