#ifndef __SINGLE_SERVER_SENTRY_LISTENER_HPP__
#define __SINGLE_SERVER_SENTRY_LISTENER_HPP__

#include <map>

#include "../../boost_network/network.hpp"
#include "../../boost_network/message.hpp"

class SentryListener : public BOOST_NETWORK::AcceptorCallback {
    const char* name() { return "sentey"; }

    void onNewAccept(const BOOST_NETWORK::TcpSocketPtr& s);

    void onNewMsg(const BOOST_NETWORK::TcpSocketPtr& s, uint16 cmd, const BOOST_NETWORK::MsgBufPtr &buf);

    void onClose(const BOOST_NETWORK::TcpSocketPtr& s);

};

#endif
