#ifndef __BOOST_NETWORK_TCP_SOCKET_HPP__
#define __BOOST_NETWORK_TCP_SOCKET_HPP__

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service_strand.hpp>

#include "buffer.hpp"
#include "io_interface.hpp"
#include "../utils/macro.hpp"
#include "../utils/noncopyable.hpp"

NS_BOOST_NETWORK_BEGIN

class TcpSocket : public std::enable_shared_from_this<TcpSocket> , private UTILS::Noncopyable {
public:
    //avoid default construct
    explicit TcpSocket(boost::asio::io_socket& m_ios, boost::asio::ip::tcp::socket socket, boost::asio::ip::tcp::endpoint& addres, IoInterface* pInterface, uint32 index = 0);
    ~TcpSocket();

public:
    // read socket
    void read();
    // write parameter cmd is protocol id, mssages , comproess
    void write(uint16 cmd, MsgBufPrt& msg, bool isCompress = false);

    void close();

public:
    int32 getSocketId() { return m_socketId;}

    uint32 getMsgIndex() { return m_msgIndex; }

    uint32 getPort() { return m_port; }

    std::string getIp() { return m_ip; }

    boost::asio::ip::tcp::socket& getSocket() { return m_socket; }

    TcpSocketPtr getSharedPtr() { return shared_from_this(); }

    IoInterface * getInerface() { return m_ioInterface; }

    void bindUid(uint64 uid) { m_bindUid = uid; }

    uint64 getBindUid() { return m_bindUid; }

    bool getCloseFlag() { return m_closeFlag; }

private:
    // socket read
    void onRead(const TcpSocketPtr& self);
    // socket write
    void onWrite(const TcpSocketPtr& self);
    // close
    void onClose(const TcpSocketPtr& self);
    //err
    void onReadError(const TcpSocketPtr& self, const boost::system::error_code& ec);

    void onWriteError(const TcpSocketPtr& self, const boost::system::error_code& ec);
    // parse
    bool readParse(const TcpSocketPtr& self, uint32 recvLength);
    // compress
    bool isNeedCompress(bool isCompressed, bool isCompress, uint32 msgSize);
    //get idle index
    uint8 getIdleIndex(uint8 index) { return index ^= 1}

private:
    boost::asio::io_service::strand m_strand;
    boost::asio::ip::tcp::socket m_socket;

private:
    DynamicBuffer m_recvBuffer;         // recve buff
    DynamicBuffer m_sendbuffer[2];      // send
    uint8 m_idleIndex = 0;              // idle index
    bool m_asyncWriting = false;        // async writing
    bool m_notifyCallback = true;       // callback

    //notes  std socket content
    int32 m_socketId = -1;              // socket id
    uint32 m_msgIndex = 0;              // msg index
    uint32 m_port = 0;                  // port
    std::string m_ip = "";              // ip

    IoInterface* m_ioInterface = NULL;

    uint64 m_bindUid = 0;               // bind uid

    bool m_closeFlag = false;
};

typedef std::shared_ptr<TcpSocket> TcpSocketPtr;

NS_BOOST_NETWORK_END
#endif
