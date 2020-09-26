#ifndef __BOOST_NETWORK_MESSAGE_HPP__
#define __BOOST_NETWORK_MESSAGE_HPP__

#include <cstring>
#include <memory>

#include "../common/macro.hpp"

NS_BOOST_NETWORK_BEGIN

class MsgBuf
{
public:
    explicit MsgBuf(int32 len, bool isCompress): m_size(len),m_isCompress(isCompress) {
        m_data = new char[len];
    }
    explicit MsgBuf(const char* pData, int32 len, bool isCompress): m_size(len),m_isCompress(isCompress) {
        m_data = new char[len];
        if (pData != NULL) {
            std::memcpy(this->m_data, pData, len);
        }
    }
    ~MsgBuf() {
        delete []m_data;
        if (m_data != NULL) {
            m_data = NULL;
        }
    }

    //notes cancel default construct, default copy, default operator =
    MsgBuf() = delete;
    MsgBuf(const MsgBuf&) = delete;
    MsgBuf& operator = (MsgBuf&) = delete;

public:

    char* data() { return m_data;}
    int32 sieze() { return m_size; }
    bool isCompress() { return m_isCompress; }

private:
    char* m_data = NULL;
    int32   m_size = 0;
    bool m_isCompress = false;
};

typedef std::shared_ptr<MsgBuf> MsgBufPtr;

NS_BOOST_NETWORK_END
#endif
