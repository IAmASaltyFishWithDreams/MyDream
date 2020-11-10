#ifndef __ROBOT_MAIN_ROBOT_HPP__
#define __ROBOT_MAIN_ROBOT_HPP__
#include "../../boost_network/message.hpp"
#include "../logic/logic_connector.hpp"
#include "../../proto/op_cmd/cmd_manager.hpp"
#include "../../service_core/boost_server.hpp"
#include "../../service_core/boost_msg_handler.hpp"

class Robot : public SERVICE_SERVER::BoostServer {
public:
    Robot(){}
    ~Robot(){}

public:
    const char* getServerName() { return "robot"; }

    bool onInitServer();

    void onRunServer();

    bool onCloseServer();

    bool onInitNetWork();

    bool onInitLogic();

public:
    void onLogicConnect(const BOOST_NETWORK::TcpSocketPtr& s);

    void onLogicMsg(const BOOST_NETWORK::TcpSocketPtr& s, uint16 cmd, const BOOST_NETWORK::MsgBufPtr& buf);

    void onLogicClose(const BOOST_NETWORK::TcpSocketPtr& s);

public:
    bool onLoadConfig();();

    bool onLoadXmlConfig();

public:
    LogicConnector m_connector;

private:
    template <typename T_MSG>
    static bool sendMsgBySocket(const BOOST_NETWORK::TcpSocketPtr& pSocket, uint16_t cmd, const T_MSG msg);
};

template <typename T_MSG>
bool sendMsgBySocket(const BOOST_NETWORK::TcpSocketPtr& pSocket, uint16_t cmd, const T_MSG msg) {
    if (pSocket == NULL) {
        log_error("send cmd [%s:%d] to server failed, pSocket == NULL", CMD_DESC(cmd), cmd);
        return false;
    }
    MSG_SERIALIZE_TO_MSGBUF(cmd,msg);

    if(msg == NULL) {
        log_error("send socket [%d] cmd [%s:%d] to server failed, msg == NULL", pSocket->getSocketId(), CMD_DESC(cmd), cmd);
        return;
    }
    log_trace("send message cmd[%s:%d]", CMD_DESC(cmd), cmd);
    pSocket.write(cmd, msg);
    return true;
}
#endif
