#include "tcp_socket.hpp"

#include <functional>
#include <boost/aios/buffer.hpp>

#include "../utils/log.hpp"
#include "../utils/scope_gourd.hpp"

NS_BOOST_NETWORK_BEGIN
USING_NS_UTILS
TcpSocket::TcpSocket(boost::asio::io_sevice& ios, boost::asio::ip::tcp::socket socket, boost::asio::ip::tcp::endpoint& addr, IoInterface* pInterface, int32 index)
    : m_strand(ios)
    , m_socket(std::move(socket))
    , m_idleIndex(0)
    , m_asyncWriting(false)
    , m_notifyCallback(true)
    , m_socketId(socket.native_handle())
    , m_msgIndex(index)
    , m_port(addr.port())
    , m_ip(addr.address().to_string())
    , m_ioInterface(pInterface)
    , m_closeFlag(false)
{
    m_socket.set_option(boost::asio::ip::tcp::no_deloy(true));
}

TcpSocket::~TcpSocket() {
    if (m_socket.is_open()) {
        boost::system::error_code ec;
        m_socket.close(ec);
        CHECK_ERROR_LOG(ec, log_error("close socket [%d], was error : %s", m_socketId, ec.message().to_str()));
    }
}

void TcpSocket::read(){
    auto self(shared_from_this());
    doRead(self);
}

void TcpSocket::write(uint16 cmd, MsgBufPtr& buf, bool isCompress) {
    auto self(shared_from_this());
    m_strand.post([this, self, cmd, buf, isCompress](){
                    if (m_closeFlag) {
                        log_error("socket [%u] , was stop write closeFlag: [%d] ", self->getSocketId(), m_closeFlag);
                        return;
                    }
                    bool needCompress = isNeedCompress(buf->isCompress(), isCompress, buf->size());
                    if (needCompress) {
                        //notes this need compress
                    } else {
                        if (buf->size() > PACKET_BODY_MAX_SIZE) {
                            log_error("Socket [%u], cmd [%u], buf size[%u] >  body max size :[%u], compress [%d]", self->getSocketId(), cmd, buf->size(), PACKET_BODY_MAX_SIZE, buf->isCompress());
                            return;
                        }
                        //notes buffer op
                        uint32 packetHeadSize = PACKET_HEAD_SIZE;
                        uint32 msgAllSize = packetHeadSize + buf->szie(); 
                        m_sendbuffer[m_idleIndex].reserve(msgAllSize);

                        if (m_sendbuffer[m_idleIndex].getWriteSize() < msgAllSize) {
                            log_error("Socket [%u], cmd [%u] write size [%u] < msg all size [%u], idle [%d]", self->getSocketId(), cmd, m_sendbuffer[m_idleIndex].getWriteSize(), msgAllSize, m_idleIndex);
                            m_sendbuffer[m_idleindex].show();
                            return;
                        }

                        MsgHead*  pHead = (MsgHead *)m_sendbuffer[m_idleIndex].getWritePos();
                        pHead->cmd = cmd;
                        pHead->len = buf->szie();
                        pHead->index = self->m_msgIndex;
                        pHead->flag = 0;
                        if (buf->isCompress()) {
                            pHead->flag = PACKET_HEAD_SIZE;
                        }
                        memcpy(&m_sendbuffer[m_idleIndex].getWritePos()[packetHeadSize], buf->data(), buf->szie());
                        m_sendbuffer[m_idleIndex].addWriteLen(packetHeadSize + buf->size());
                    }
                    doWrite(self);
                  });
}

void TcpSocket::close() {
    auto self(shared_from_this());
    m_strand.post([this,self](){
                  m_closeFlag = true;
                  //if not asyncWriting please close
                  if (!m_asyncWriting) {
                    m_notifyCallback = false;
                    self->doClose(self);
                  }
                  });
}

void TcpSocket::doRead(const TcpSocketPtr& self) {
    //boost descriptor.async_read_some(boost::asio::buffer(data, size), handler);
    m_socket.async_read_some(boost::asio::buffer(m_recvBuffer.getWritePos(), m_recvBuffer.getWriteSize()), 
                   m_strand.wrap([this,self](boost::system::error_code ec,std::size_t recvLength){
                                 do {
                                      if(m_closeFlag) {
                                          log_warn("Socket [%u] read stop close [%d] ",self->getSocketId(), m_closeFlag);
                                          return;
                                      }

                                      if (ec) {
                                          onReadError(self,ec);
                                          return;
                                      }

                                      if(readParse(self,recvLength) == false ) {
                                          onReadError(self,boost::system::errc::make_error_code(boost::system::errc::bad_massage));
                                          return;
                                      }
                                      doRead(self);
                                 } while(0);
             }));
}

void TcpSocket::doWrite(const TcpSocketPtr& self) {
    //async write once
    if (m_asyncWriting) {
        return;
    }
    //not data need write
    if (m_sendbuffer[m_idleIndex].isReadOver()) {
        if (m_closeFlag) {
            m_strand.post([this,self](){ m_notifyCallback = false;  doClose(self);  })
        }
        return;
    }

    m_asyncWriting = true;
    uint8 asyncWriteIndex = m_idleIndex;
    m_idleIndex = getIdleIndex(m_idleIndex);
    //nost boost  socket.async_send(boost::asio::buffer(data, size), handler)
    m_socket.async_send(boost::asio::buffer(m_sendbuffer[asyncWriteIndex].getReadPos(),m_sendbuffer[asyncWriteIndex].getReadSize()),
                        m_strand.wrap([this,self](boos::system::error_code& ec, std::size_t sendLeagth){
                                      if(ec) {
                                            onWriteError(self,ec);
                                            return;
                                      }

                                      uint8 asyncWriteIndex = getIdleIndex(m_idleIndex);
                                      m_sendbuffer[asyncWriteIndex].addReadLen(m_sendbuffer[asyncWriteIndex].getReadSize());
                                      m_sendbuffer[asyncWriteIndex].refitBuffer();
                                      m_asyncWriting = false;

                                      doWrite(self);
                                      }));
}

void TcpSocket::doClose(const TcpSocketPtr& self) {
    if (m_socket.is_open()) {
        log_debug("Close socket [%d] addr [%s:%u]", m_socketId, getIp().c_str(), getPort());
        boost::system::error_code ec;

        m_socket.cancel();
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
        CHECK_ERROR_LOG(ec, "Shutdown socket [%d] was error [%s]", m_socketId, ec.message().c_str());
        m_socket.close();
        CHECK_ERROR_LOG(ec, "Close socket [%d] was error [%s]", m_socketId, ec.message().c_str());
        //call back
        if (m_notifyCallback) {
            m_ioInterface->onPassiveSocketClose(self);
        }
    }
}

void TcpSocket::onReadError(const TcpSocketPtr& self, const boost::system::error_code& ec) {
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }
    m_ioInterface->onReadError(self, ec);
    doClose(self);
}

void TcpSocket::onWriteError(const TcpSocketPtr& self, const boost::system::error_code& ec) {
    if (ec == boost::asio::error::operation_aborted) {
        return;
    }
    m_ioInterface->onWriteError(self,ec);
    doClose(self);
}

bool TcpSocket::readParse(const TcpSocketPtr& self, uint32 recvLength) {
    m_recvBuffer.addWriteLen(recvLength);
    uint32 recvCount = 0;
    auto guard = NewScopeGuard([this, &recvCount](){
                               if (recvCount > 0) {
                                     m_recvBuffer.refitBuffer();
                               }
                        });
    do {
        //check cache
        if (m_recvBuffer.getReadSize() > PACKET_HEAD_SIZE) {
            break;
        }

        MsgHead* pHead = (MsgHead*) m_recvBuffer.getReadPos();
        uint32 msgSize = PACKET_HEAD_SIZE + pHead->len;
        if (msgSize > PACKET_MAX_SIZE) {
            log_error("Socket [%u] recv packget size [%u] > max size [%u]", m_socketId, msgSize, PACKET_MAX_SIZE);
            return false;
        }

        if (msgSize > m_recvBuffer.getReadSize()) {
            break;
        }
        //construct msg obj
        MsgBufPtr pMsgBuf = std::shared_ptr<MsgBufPtr>(&(m_recvBuffer.getReadPos().[PACKET_MAX_SIZE]), pHead->len, PACKET_FLAG_ZIP == (pHead->flag & PACKET_FLAG_ZIP));

        if (pHead->index > 0 && self->m_msgIndex == 0) {
            self->m_msgIndex = pHead->index;
        }

        // self->msgIndex 0 sever communication not need check
        // count check
        // onNewMsg 重点这个函数相当于servlet 接过去后就可以用josn等等方式进行解析
        if (self->msgIndex == 0) {
            m_ioInterface->onNewMsg(self, pHead->cmd, pMsgBuf);
        } else if (pHead->index == self->m_msgIndex) {
            m_ioInterface->onNewMsg(self, pHead->cmd, pMsgBuf);
            self->m_msgIndex += 1;
            if (self->m_msgIndex == 0xFF) {
                self->m_msgIndex = 1;
            }
        } else {
            log_error("Socket [%u], cmd [%u] cliet msg index [%u] != server msg index[%u]", m_socketId, pHead->cmd, pHead->index, self->m_msgIndex);
            return false;
        }
        m_recvBuffer.addReadLen(msgSize);
        recvCount++;

    }while(true);
    return true;
}

bool TcpSocket::isNeedCompress(bool isCompressed, bool isCompress, uint32 msgSize) {
    //read module not compress so special operation
    return false;

    if (isCompressed) {
        return false;
    }
    if (msgSize > PACKET_BODY_MAX_SIZE) {
        return true;
    }
    if (msgSize >= PACKET_COMPRESS_MIN_SIZE && isCompress) {
        return true;
    }
     return false;
}

NS_BOOST_NETWORK_END
