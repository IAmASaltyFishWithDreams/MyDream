#include "boost_server.h"
#include "../utils/log.hpp"

NS_SERVICE_CORE_BEGIN
//TODO inherit core server

bool Boost_server::startNetwork(uint32 nthread) {
    return m_network.init(nthread);
}

bool Boost_server::listenAt(const std::string& ip, uint16 port, AcceptorCallback* pAcceptorCb) {
    return m_network.listenAt(ip,port,pAcceptorCb);
}

bool Boost_server::connectTo(const std::string& ip, uint16 port, ConnectorCallback* pConnectorCb, uint32 reConnectTime) {
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

NS_SERVICE_CORE_END
