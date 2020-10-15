#include "network.hpp"

#include "boost/bind.hpp"
#include "../utils/log.hpp"

NS_BOOST_NETWORK_BEGIN

void BoostNetwork::init(uint32 threadNum) {
    if (threadNum == 0) {
        log_error("threadNum must > 0, please check threadNum");
    }
    m_workPtr.reset(new boost::asio::io_service::work(m_ios));
    m_threadPool.create_thread(boost::bind(&boost::asio::io_service::run,&m_ios),threadNum);
}

void BoostNetwork::close() {
    clearListen();
    clearConnect();

    m_workPtr.reset();
    m_threadPool.join();
    //notes why solve 2020.10.15 clearListen() clear inside; m_connectVec.clear() clear exteral
    m_connectVec.clear();
    m_lisetenMap.clear();
}

void BoostNetwork::stop() {
    stopListen();
    stopConnect();
}

bool BoostNetwork::listenAt(const std::string& ip, uint16 port, AcceptorCallback* pAcceptorCb) {
    if (m_lisetenMap.find(port) != m_lisetenMap.end()) {
        log.error("port[%u] already lisenten", port);
        return false;
    }

    if (pAcceptorCb == NULL) {
        log.error("pAcceptorCb == NULL");
        return false;
    }

    //start lisent port
   AcceptorPtr acceptor = std::make_shared<Acceptor>(m_ios, ip, port, pAcceptorCb);
   if (!acceptor->lisent()) {
       log_error("lisent failure ip:%s:%u", ip.c_str(),port);
       return false;
   }
   m_lisetenMap.insert(std::make_pair(port,pAcceptorCb));
   return true;
}

bool BoostNetwork::connectTo(const std::string& ip, uint16 port, ConnectorCallback* pConnectCb, int32 reConnectTime) {
    if (pConnectCb == NULL) {
        log_error("pConnectCb == NILL");
        return false;
    }
    //start connect
    ConnectorPtr pConnectPtr = std::make_shared<ConnectorPtr>(new Connector(m_ios,ip,port,pConnectCb,reConnectTime));
    m_connectVec.push_back(pConnectPtr);
    return true;
}

void BoostNetwor::postCloseSocket(const TcpSocketPtr& s) {
    if (s == NULL) {
        log_error("TcoSocketPtr == NULL");
        return
    }
    IoInterface* pInterface = s->getIoInterface();
    if (pInterface == NULL){
        log_error("pInterface == NULL at socket [%d][%s:%d]", s->getSocketId(), s->getIp().c_str(), s->getPort());
        return;
    }
    //active socket close
    pInterface->onActiveSocketClose(s);
}

void BoostNetwork::stopListen() {
    for (const auto& iter : m_lisetenMap) {
        if (iter){
            iter->stop();
        }
    }
}

void BoostNetwork::clearListen() {
    for(const auto& iter : m_lisetenMap) {
        if (iter) {
            iter->clear();
        }
    }
}

void BoostNetwork::stopConnect() {
    for(const auto& iter : m_connectVec) {
        if (iter) {
            iter->stop();
        }
    }
}

void BoostNetwork::clearConnect() {
    for(const auto& iter : m_connectVec) {
        if (iter) {
            iter->clear();
        }
    }
}

NS_BOOST_NETWORK_END
