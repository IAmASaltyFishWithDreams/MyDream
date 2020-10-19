#ifndef __SERVICE_CORE_BOOST_SEVER_HPP__
#define __SERVICE_CORE_BOOST_SEVER_HPP__

#include "base_server.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>

#include "../boost_network/network.hpp"

NS_SERVICE_CORE_BEGIN
/**
 *  three layer server
*/
class BoostServer : public BaseServer {
public:
    Boost_server() : m_ios(),m_singleWait(m_ios) {}
    ~Boost_server() {}

public:
    virtual char* getServerName() = 0;
    virtual bool onInitStart() = 0;
    virtual void onRunServer() = 0;
    virtual bool onCloseServer() = 0;

public:
    bool initServer();;
    bool runServer();
    bool closeServer();
    bool initSignal();

    void interruptedServer();

public:
    /*network layer*/
    bool startNetwork(uint32 nthread);

    bool listenAt(const std::string& ip, uint16 port, BOOST_NETWORK::AcceptorCallback* pAcceptorCb);

    bool connectTo(const std::string& ip, uint16 port,  BOOST_NETWORK::ConnectorCallback* pConnectorCb, uint32 reConnectTime);

    void stopNetwork();

    void postCloseSocket(const TcpSocketPtr& ptr);

public:
    boost::asio::io_service& getIos() { return m_ios; }

private:
    boost::asio::io_service  m_ios;                         // server obj
    boost::asio::signal_set  m_singleWait;                  // catch single (RAII desconstruct call handle)
    std::shared_ptr<boost::asio::io_service::work> m_work;  // server control

private:
    /*net data*/
    BOOST_NETWORK::BoostNetwork m_network;
};

NS_SERVICE_CORE_END
#endif
