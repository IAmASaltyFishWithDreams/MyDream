#include "connector.hpp"

#include <boost/asio/ip/address.h>

#include "tcp_socket.hpp"
#include "../utils/log.hpp"

NS_BOOST_NETWORK_BEGIN
USING_NS_UTILS

Connector::Connector(const boost::asio::io_service& ios, const std::string& ip, uint16 port, IoCallback* pCallback, uint32 reconnectTime)
    : IoInterface(ios,pCallback)
    , m_strand(ios)
    , m_newSocket(ios)
    , m_socketPtr(NULL)
    , m_ip(ip)
    , m_port(port)
    , m_reconnectTime(reconnectTime)
    , m_startSign(false)
    , m_timer(ios){
        connect();
}

void Connector::onReadError(const TcpSocketPtr& s, boost::system::error_code& ec) {
    if (!s) {
        log_error("s == NULL");
        return;
    }
    log_error("[%s], disconnect to socket [%u][%s:%u], error [%d:%s]", m_ioCallback->name(), s->getSocketId(), getIp().c_str, m_port, ec.value(), ec.message().t_str());
}

void Connector::onWriteError(const TcpSocketPtr& s, boost::system::error_code& ec) {
    if (!s) {
        log_error("s == NULL");
        return;
    }
    log_error("[%s] disconnect to socket [%u][%s:%u], error[%d:%s]", m_ioCallback->name(), s->getSocketId(), getIp().c_str(), m_port, ec.value(), ec.message().c_str());
}

void Connector::onActiveSocketClose(const TcpSocketPtr& s) {
    if (!s) {
        log_error("s == NULL");
        return;
    }
    m_strand.post([this,s](){
                  if (!m_socketPtr) {
                    return;
                  }
                  if (m_socketPtr != s) {
                    log_error(" sockerPtr != s");
                    return;
                  }
                  m_socketPtr->close();
                  m_socketPtr.reset();

                  reconnect();
                  });
}

void Connector::onPassiveSocketClose(const TcpSocketPtr& s) {
    if(!s) {
        log_error("s == NULL");
        return;
    }
    m_strand.post([this,s](){
                  if (!m_socketPtr){
                    return;
                  }
                  if (m_socketPtr != s) {
                    log_error("m_socket != s");
                    return;
                  }
                  m_ioCallback->onClose(s);
                  m_socketPtr.reset();

                  reconnect();
                  });
}

void Connector::connect() {
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::address::from_string(m_ip), m_port);
    m_newSocket.async_connect(ep,m_strand([this,ep](boost::system::error_code& ec){
                              if (ec) {
                                log_error("[%s], connect socket [%u][%s:%u] failure error : [%d:%s]", m_ioCallback.name(), m_newSocket.native_handle(), ep.address().to_string().t_str(), ep.prot(), ec.value(), ec.message().c_str());
                                boost::system::error_code ignoral_ec;

                                m_newSocke.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignoral_ec);
                                m_newSocket.close(ignoral_ec);

                                if (ec == boost::asio::error::operation_abroted) {
                                    return;
                                }
                                reconnect();
                              } else {
                                log_trace("[%s, connect sokcet[%u][%s:%u] succeed", m_ioCallback.name(),m_newSocket.native_handle(), ep.address().to_string().c_str(), ep.port());
                                //notes 重点这里是进行socket连接的地方
                                m_socketPtr.reset(new TcpSocketPtr(m_ios, std::move(m_newSocket), ep, this));
                                m_ioCallbacke.onConnect(m_socketPtr);:
                                m_socketPtr.read();
                              }
    }));
}

void Connector::reconnect() {
    if(m_reconnectTime == 0) {
        return;
    }
    if(!m_startSign){
        return;
    }

    m_timer.expirse_from_now(boost::posix_time::seconds(m_reconnectTime));
    m_timer.async_waite(m_strad.wrap([this](boost::system::error_code& ec){
                                     if (ec == boost::asio::error::operation_aborted) {
                                        return;
                                    } else if(ec) {
                                       log_error("Time error : %s", ec.message().c_str()) ;
                                    }
                                    connect();
                                    log_trace("[%s] start reconnect socket [%s:%u]", m_ioCallback.neme(), m_ip.c_str(), m_port);
            }));
}

void Connector::stop(){
    m_stand.post([this](){
                 //stoping : startSign set, timer cancel
                 m_startSign = false;
                 if(!m_socketPtr){
                    boost::system::error_code ec;
                    m_newSocket.cancel(ec);
                    CHECK_ERROR_LOG(ec, log_error("[%s] close  socket[%s:%u] failed ", m_ioCallback.name(),m_ip.c_str(), m_port));
                 }
                 m_timer.cancel();
                 });
}

void Connector::clear(){
    m_strand.post([this](){
                  // if connectint first close,then reset
                  if(m_socketPtr) {
                    m_socketPtr->close();
                    m_socketPtr.reset();
                  }
                  });
}

NS_BOOST_NETWORK_END

