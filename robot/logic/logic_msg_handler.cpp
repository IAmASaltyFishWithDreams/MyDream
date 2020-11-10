#include "logic_msg_handler.hpp"

#include "../main/robot.hpp"
#include "../main/robot_global.hpp"
#include "protocpp/op_cmd.hpp"
#include "protocpp/protos/server.bp.h"

SERVICE_CORE::BoostMsgHandler LogicMsgHandler::s_msgHander;

bool LogicMsgHandler::initMsgHandler() {
    bool ret = true;
    ret &= s_msgHander.registerFunc(PROTOCPP::MsgPing, handleMsgPing);
    ret &= s_msgHander.registerFunc(PROTOCPP::MsgTestTcpReply, handleMsgTestTcpReply);
    return ret;
}

bool LogicMsgHandler::handleMsgPing(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf) {
    MSG_DESERIALIZE_BY_MSGBUF(PROTOCPP::MsgPing, proto::server::MsgPing, buf);

    proto::server::MsgPong reply;

    global::pRobot->sendMsgBySocket(s, PROTOCPP::MsgPong, reply);
    return true;
}

bool LogicMsgHandler::handleMsgTestTcpReply(const BOOST_NETWORK::TcoSocketPtr& s, const BOOST_NETWORK::MsgBuf& buf) {
    MSG_DESERIALIZE_BY_MSGBUF(PROTOCPP::MsgTestTcpReply, proto::server::MsgTestTcpReply, buf);

    log_debug("robot test tcp messages ...");
    return true;
}
