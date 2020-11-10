#ifndef __ROTO_LOGIC_MSG_HANDLER_HPP__
#define __ROTO_LOGIC_MSG_HANDLER_HPP__

#include "protocpp/cmd_manager.hpp"
#include "../../service_core/boost_msg_handler.hpp"

class LogicMsgHandler {
public:
    static bool initMsgHandler();

    static bool handleMsgPing(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf);

    static bool handleMsgTestTcpReply(const BOOST_NETWORK::TcpSocketPtr& s, const BOOST_NETWORK::MsgBufPtr& buf);

private:
    static SERVICE_CORE::BoostMsgHandler s_msgHandler;
};

#endif
