#include "sentry_msg_handler.hpp"

#include "protocpp/cmd_manager.hpp"
#include "protocpp/protos/server.bp.h"
#include "sentry_manager.hpp"

bool SentryMsgHandler::initMsgHandler() {
    bool result = true;
    result &= s_msgHander.registerFunc(PROTOCPP::MsgPong, handleMsgPong());
    result &= s_msgHander.registerFunc(PROTOCPP::MsgRobot, handleMsgRobot());
    result &= s_msgHander.registerFunc(PROTOCPP::MsgTestTcpRequest, handleMsgTestTcpRequest());
    return result;
}

bool SentryMsgHandler::handleMsgRobot(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf) {
    auto pOb = ObjectManager::findObject(s->getBindUid());
    if (pOb == NULL) {
        log_error("pOb == NULL, socket [%d][%s:%u]", s->getSocketId(), s->getIp(), s->getPort());
        return false;
    }
    MSG_DESERIALIZE_BY_MSGBUF(PROTOCPP::MsgRobot, proto::server::MsgRobot, buf);
    //log_info("Robot [%u] Recv [%s]", pOb->getBindUid(), msg.test().c_str()); //problem 2020.10.29 why msg?
    return true;
}

bool SentryMsgHandler::handleMsgPong(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf) {
    auto pOb = ObjectManager::findObject(s->getBindUid());
    if (pOb == NULL) {
        log_error(" pOb == NULL, socket [%d][%s:%u]", s->getSocketId(), s->getIp(), s->getPort());
        return false;
    }
    MSG_DESERIALIZE_BY_MSGBUF(PROTOCPP::MsgPong, proto::server::MsgPong, buf);
    pOb->setActive();
    return true;
}

bool SentryMsgHandler::handleMsgTestTcpRequest(const BOOST_NETWORK::TcpSocketPtr& s, const  BOOST_NETWORK::MsgBufPtr& buf) {
    auto pOb = ObjectManager::fandObject(s->getBindUid());
    if (pOb == NULL ) {
        log_error(" pOb == NULL, socket [%d][%s:%u]", s->getSocketId(), s->getIp(), s->getPort());
        return false;
    }
    MSG_DESERIALIZE_BY_MSGBUF(PROTOCPP::MsgTestTcpRequest, proto::server::MsgTestTcpRequest, buf);
    pOb->testLimitTcp(msg.count()); // msg ?
    return true;
}
