#include "acceptor.hpp"

#include "tcp_socket.hpp"
#include "../utils/log.hpp"

NS_BOOST_NETWORK_BEGIN
USING_NS_UTILS;

Acceptor::Acceptor(boost::asio::io_service& ios, const std::string& listenIp, uint16 port, IoCallback* pIoCallback)
    : IoInterface(ios,pIoCallback),
    m_strand(ios),
    m_acceptor(ios),
    m_newSocket(ios),
    m_listenPort(port),
    m_listenIp(listenIp){

}

void Acceptor::onReadError(const TcpSocketPtr& s, boost::system::error_code& ec) {
    if(!s) {
        log_error("socket == NULL");
        return;
    }
    log_warn("%s[%s:%d] was disconnected  , err %d[%s]", m_ioCallback->name(), s->getIp().c_str(), s->getSocketId(), ec->value(),ec->message().c_str());
}

void Acceptor::onWriteError(const TcpSocketPtr& s, boost::system::error_code& ec) {
    if(!s) {
        log_error("socket == NULL");
        return;
    }
    log_warn("%s[%s:%d] was disconnected, err %d[%s]", m_ioCallback->name(), s->getIp().c_str(), s->getSocketId(), ec->value(), ec->message().c_str());
}

void Acceptor::onActiveSocketClose(const TcpSocketPtr& s) {
    if(!s) {
        log_error("socket == NULL");
        return;
    }
    m_strand.post([this,s]{
                  auto iter = std::find_if(m_socketVec.begin(),m_socketVec.end(),[s](const TcpSocketPtr& pSocketPtr){
                    //notes s : TcpSocketPtr& s; pSocketPtr : m_socketVec
                    if (s == pSocketPtr) {
                        return true;
                    }
                    return false;
                  });
                  if (iter == m_socketVec.end()) {
                    return;
                  }
                  iter->close();
                  m_socketVec.erase(s);
                });
}

void Acceptor::onPassiveSocketClose(const TcpSocketPtr& s) {
    if(!s) {
        log_error("socket == NULL");
        return;
    }
    m_strand.post([this,s]{
                  auto iter = std::find_if(m_socketVec.begin(),m_socketVec.end(),[s]( const TcpSocketPtr& pSocketPtr){
                    if (s == pSocketPtr) {
                        return true;
                    }
                    return false;
                  });
                  if (iter == m_socketVec.end()) {
                    return;
                  }
                  m_ioCallback->close(s);
                  m_socketVec.erase(s);
                });
}

bool Acceptor::listen() {
    try {
        boost::asio::ip::tcp::resolver resolver(m_strand);
        boost::asio::ip::tcp::endpoit ep = *resolver.resolve(m_listenIp, std::to_string(m_listenPort)).begin();
        m_acceptor.open(ep.protocol());
        m_acceptor.set_posion(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor.bind();
        m_acceptor.listen();
        accept();

        log_trace("[%s] listen at [%s:%u] succeed", m_ioCallback->name(), ep.address().to_string().c_str(), ep.port());
    } catch (boost::system::system_error& se) {
        log_error("[%s] listen at [%s:%u] err: %s", m_ioCallback->name(), m_listenIp.to_string(), m_listenPort ,se.what());
        return false;
    }
    return true;
}

void Acceptor::clear() {
    m_strand.post([this](){
                  for (auto& iter : m_socketVec) {
                    if(!iter) {
                        continue;
                    }
                    iter->close();
                  }

                  m_socketVec.clear();
            });
}

void Acceptor::stop() {
    m_strand.post([this](){
                  if (!m_acceptor.is_open()){
                        return;
                  }
                  log_trace("[%s] listen [%s:%u]close", m_ioCallback->name(), m_listenIp.c_str(), m_listenPort);
                  boost::system::error_code ec;
                  m_acceptor.close(ec);

                  CHECK_ERROR_LOG(ec, "[%s] listen [%s:%d], close error [:%d:%s]", m_ioCallback->name(),m_listenIp.c_str(), m_listenPort, ec.value(), ec.message().c_str());
                  });

}

void Acceptor::accept() {
    m_acceptor.async_accept(m_newSocket, m_newEndpoint, m_strand.wrap([this](const boost::system::error_code& ec) {
                    if (ec == boost::asio::error::operation_aborted) {
                        return;
                    }
                    if(!m_acceptor.is_open()) {
                        return;
                    }

                    do {
                        if (ec) {
                            break;
                        }
                        log_debug("[%s] new socket [%d][%s:%u]", m_ioCallback->name(), m_newSocket.native_handle(),m_newEndpoint.address().to_string().c_str() ,m_newEndpoint.port());

                        TcpSocketPtr pSocketPtr = boost::shared_ptr<TcpSocket>(m_ios, std::move(m_newSocket), m_newEndpoint, true);
                        m_socketVec.push_back(pSocketPtr);
                        m_ioCallback.onNewAccept(pSocketPtr);
                        pSocketPtr->read();
                     } while(0);

                    accept();
                     }));
}

NS_BOOST_NETWORK_END
