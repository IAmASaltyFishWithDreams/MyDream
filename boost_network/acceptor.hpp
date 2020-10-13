#ifndef __BOOST_NETWORK_ACCEPTOR_HPP__
#define __BOOST_NETWORK_ACCEPTOR_HPP__

#include <string>
#include <vector>

#include <boost/asio/io_service_strand.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "io_interface.hpp"
#include "../utils/macro.hpp"
#include "../utils/noncopyable.hpp"

NS_BOOST_NETWORK_BEGIN

class Acceptor : public IoInterface
                 ,public std::enable_shared_from_this<Acceptor>
                 ,public UTILS::Noncopyable
{
public:
    //notes explicit cconstruct
    explicit Acceptor(const boost::asio::io_service& ios, const std::string& listenIp, uint16 port, IoCallback* pIoCallback);

public:

    void onReadError(const TcpSocketPtr& s, boost::system::error_code& ec);

    void onWriteError(const TcpSocketPtr& s, boost::system::error_code& ec);

    void onActiveSocketClose(const TcpSocketPtr& s);

    void onPassiveSocketClose(const TcpSocketPtr& s);

public:
    // start listen ip
    bool listen();
    //clear all listen
    void clear();
    //stop apcceptor
    void stop();

private:
    //asyn deal with 'connect'
    void accept();

private:
    uint16 m_listenPort;
    std::string m_listenIp;
    std::vector<TcpSocketPtr> m_socketVec;

    boost::asio::io_service::strand m_strand;
    boost::asio::ip::tcp::acceptor  m_acceptor;
    boost::asio::ip::tcp::socket    m_newSocket;
    boost::asio::ip::tcp::endpoint  m_newEndpoint;
};

typedef  std::shared_ptr<Acceptor> AcceptorPtr;

NS_BOOST_NETWORK_END
#endif
