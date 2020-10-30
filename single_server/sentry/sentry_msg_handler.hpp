#ifndef __SINGLE_SERVER_SENTRY_MSG_HANDLER_HPP__
#define __SINGLE_SERVER_SENTRY_MSG_HANDLER_HPP__

#include "protocpp/cmd_manager.hpp"
#include "../../service_core/boost_msg_handler.hpp"

class SentryMsgHandler {
public:
    static bool initMsgHandler();

    static bool handleMsgRobot(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf);

    static bool handleMsgPong(const BOOST_NETWORK::TcpSocketPtr& s ,const BOOST_NETWORK::MsgBufPtr& buf);

    static bool handleMsgTestTcpRequest(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf);

private:
    static SERVICE_CORE::BoostMsgHandler s_msgHander;
};

#endif
