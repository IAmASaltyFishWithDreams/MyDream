#ifndef __ROBOT_LOGIC_CONNECTOR_HPP__
#define __ROBOT_LOGIC_CONNECTOR_HPP__

#include <map>

#include "../../boost_network/network.hpp"
#include "../../boost_network/message.hpp"

class LogicConnector : public BOOST_NETWORK::ConnectorCallback {
public:
    const char* getName() { return "logic"; }

    void onNewMsg(const BOOST_NETWORK::TcpSocketPtr& s, uint16 cmd, const BOOST_NETWORK::MsgBufPtr& buf);

    void onConnect(const BOOST_NETWORK::TcpSocketPtr& s);

    void onClose(const BOOST_NETWORK::TcpSocketPtr& s);
};
#endif
