#include <iostream>
#include "boost_server.h"
USING_BOOST_SERVER_BEGIN
//TODO inherit core server

bool Boost_server::startNetwork(int nthread) {
    return m_network.init(nthread);
}

bool Boost_server::listenAt(const std::string& ip, int port, AcceptorCallback* pAcceptorCb) {
    return m_network.listenAt(ip,port,pAcceptorCb);
}

bool Boost_server::connectTo(const std::string& ip, int port, ConnectorCallback* pConnectorCb, int reConnectTime) {
    return m_network.connectTo(ip, port, pConnectorCb, reConnectTime);
}

void Boost_server::stopNetwork() {
    /*first stop then close*/
    m_network.stop();
    m_network.close();
}

void Boost_server::postCloseSocket(const TcpSocketPtr& ptr) {
    m_network.postCloseSocket(ptr);
}

USING_BOOST_SERVER_END
