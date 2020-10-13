#ifndef __BOOST_NETWORK_IO_INTERFACE_HPP__
#define __BOOST_NETWORK_IO_INTERFACE_HPP__

#include <memory>

#include <boost/asio/io_service.hpp>

#include "message.hpp"

NS_BOOST_NETWORK_BEGIN

class TcpSocket;
typedef std::shared_ptr<TcpSocket> TcpSocketPtr;

// base class ADT
class IoCallback {
public:
    IoCallback(){}
    virtual ~IoCallback(){}

    virtual const char* name() = 0;

    virtual void onNewAccept(const TcpSocketPtr& s) = 0;
    virtual void onNewMsg(const TcpSocketPtr& s, uint16 cmd, const MsgBufPtr& buf) = 0;
    virtual void onConnect(const TcpSocketPtr& s) = 0;
    virtual void onClose(const TcpSocketPtr& s) = 0;
};

//Acceptor callback
class AcceptorCallback : public IoCallback {
public:
    AcceptorCallback(){}
    ~AcceptorCallback() {}

    virtual const char* name() = 0;
    virtual void onNewAccept(const TcpSocketPtr& s) = 0;
    virtual void onNewMsg(const TcpSocketPtr& s, uint16 cmd, const MsgBufPtr& buf) = 0;
    virtual void onClose(const TcpSocketPtr& s) = 0;
    //notes not need onConnect fnc
    void onConnect(const TcpSocketPtr& s) {}

};
//notes connect call back
class ConnectorCallback : public IoCallback {
public:
    ConnectorCallback() {}
    ~ConnectorCallback() {}

    virtual const char* name() = 0;
    virtual void onNewMsg(const TcpSocketPtr& s, uint16 cmd, const MsgBufPtr& buf) = 0;
    virtual void onConnect(const TcpSocketPtr& s) = 0;
    virtual void onClose(const TcpSocketPtr& s) = 0;
    //notes not need accept
    void onNewAccept(const TcpSocketPtr& s) {}
};
//notes ADT IO interface
class IoInterface {
public:
    explicit IoInterface(boost::asio::io_service& ios, IoCallback* pIoCallback): m_ios(ios), m_ioCallback(pIoCallback) {}
    ~IoInterface() {}

public:
    virtual void onReadError(const TcpSocketPtr& s, const boost::system::error_code& ec) = 0;
    virtual void onWriteError(const TcpSocketPtr& s, const boost::system::error_code& ec) = 0;
    virtual void onActiveSocketClose(const TcpSocketPtr& s) = 0 ;
    virtual void onPassiveSocketClose(const TcpSocketPtr& s) = 0;

    void onNewMsg(const TcpSocketPtr& s, uint16 cmd, const MsgBufPtr& buf) {
        m_ioCallback->onNewMsg(s,cmd,buf);
    }

protected:
    boost::asio::io_service& m_ios;
    IoCallback* m_ioCallback;

};

NS_BOOST_NETWORK_END
#endif
