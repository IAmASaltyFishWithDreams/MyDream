#ifndef __SINGLE_SERVER_MAIN_SINGLE_SERVER_HPP__
#define __SINGLE_SERVER_MAIN_SINGLE_SERVER_HPP__
#include "../../proto/op_cmd/cmd_manager.hpp"
#include "../sentry/sentry_lisent.hpp"
#include "../../service_core/boost_server.hpp"
#include "../../service_core/boost_msg_handler.hpp"

class SingleServer : public SERVICE_CORE::BoostServer {
public:
    char* getServerName() { return "SingleServer"; }

public:
    bool onInitStart();
    void onRunServer();
    bool onCloseServer();
    bool onInitNetWork();
    bool onInitLogic();

public:
    bool closeLogic();

public:
    void onAddSentry(const BOOST_NETWORK::TcpSocketPtr& s);
    void onDelSentry(const BOOST_NETWORK::TcpSocketPtr& s);
    void onSentryMsg(const BOOST_NETWORK::TcpSocketPtr& s, uint16 cmd, BOOST_NETWORK::MsgBufPtr& msg);

private:
    bool onLoadConfig();
    bool onLoadXmlConfig();

private:
    SentryListener m_sentryListner;

public:
    template <typename T_MSG>
    static bool sendMsgBySocket(const BOOST_NETWORK::TcpSocketPtr& pSocket, uint16 cmd, const T_MSG& msg);
};

template <typename T_MSG>
bool SingleServer::sendMsgBySocket(const BOOST_NETWORK::TcpSocketPtr& pSocket, uint16 cmd, const T_MSG& msg) {
    if (pSocket == NULL) {
        log_error("send cmd : [%s:%d] to server failed, psocket == NULL", CMD_DESC(cmd), cmd);
        return false;
    }
    MSG_SERIALIZE_TO_MSGBUF(cmd,msg);

    if (msg == NULL) {
        log_error("send socket : [%d], cmd [%s:%d], to servr failed , msg == NULL", pSocket->getSocketId(), CMD_DESC(cmd), cmd);
        return false;
    }

    log_trac("send message cmd [%s:%d] ",CMD_DESC(cmd), cmd);
    pSocket->write(cmd,msg);
    return true;
}

#endif
