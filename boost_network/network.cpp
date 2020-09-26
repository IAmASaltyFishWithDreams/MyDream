#include "network.hpp"

#include "boost/bind.hpp"
#include "../utils/log.hpp"

NS_BOOST_NETWORK_BEGIN

void BoostNetwork::init(uint32 threadNum) {
    if (threadNum == 0) {
        log_error("threadNum must > 0, please check threadNum");
    }
    m_workPtr.reset(new boost::asio::io_service::work(m_ios));
    m_threadPool.create_thread(boost::bind(boost::asio::io_service::run,&m_ios),threadNum);
}

void BoostNetwork::close() {
    clearListen();
    clearConnect();

    m_workPtr.reset();
    m_threadPool.join();
    //notes why
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

bool BoostNetwork::connectTo(const std::string& ip, uint16 port, AcceptorCallback* pAcceptorCb, int32 reConnectTime) {
    //TODO to be completed
    return true;
}

void postCloseSocket(const TcpSocketPtr& s) {
    //TODO to be completed
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
