#include "sentry_listener.hpp"

#include "../main/server_global.hpp"
#include "../main/single_server.hpp"
#include "../../utils/log.hpp"
#include "../../utils/macro.hpp"

USING_NS_BOOST_NETWORK

//network layer -> logic layer
//problem: bind ? SingleServer::onAddSentry,s, global::pServer why not?
void SentryListener::onNewAccept(const TcpSocketPtr& s) {
    global::pServer->getIos().post(std::bind(&SingleServer::onAddSentry,global::pServer,s));
}

void SentryListener::onNewMsg(const TcpSocketPtr& s, uint16 cmd, const MsgBufPtr& buf) {
    global::pServer->getIos().post(std::bind(&SingleServr::onSentryMsg, global::pServer, s, cmd, buf));
}

void SentryListener::onClose(const TcpSocketPtr& s) {
    global::pServer->getIos(.post(std::bind(&SingleServer::onDelSentry, global::pServer,s)));
}
