#ifndef __BOOST_NETWORK_BUFFER_HPP__
#define __BOOST_NETWORK_BUFFER_HPP__

#include <cstring>
#include <vector>

#include "common.hpp"
#include "../utils/log.hpp"
#include "../utils/macro.hpp"

NS_BOOST_NETWORK_BEGIN

// data cache module
/* read && write , not safe pthread
 * suitale boost::asio::strand use
 * data sequential, buffer op safe
 * */

template <typename bufferType>
class Buffer {
public:
    Buffer() {}
    ~Buffer() {}

public:
    //reset buffer stize
    void reserve(uint32 size);

public:
    //get buffer can write pos(allow write pos)
    char* getWritePos() { return &(m_buffer[m_rearIndex]); }
    //get buffer write size
    uint32 getWriteSize() { return m_maxSize - m_rearIndex; }
    //add write len
    void addWriteLen(uint32 size) { m_rearIndex += size; }

    //ditto
    char* getReadPos() { return &(m_buffer[m_curIndex]); }
    uint32 getReadSize() { return m_rearIndex - m_curIndex; }
    void addReadLen(uint32 size) { m_curIndex += size; }
    //justice read is not over(read whole data)
    bool isReadOver() { return m_curIndex >= m_rearIndex; }

    void refitBuffer() {
        if (isReadOver()) {
            m_curIndex = 0;
            m_rearIndex = 0;
        } else {
            uint32 diff = m_rearIndex - m_curIndex;
            std::memmove(&m_buffer[0], m_buffer[m_curIndex], diff);
            m_curIndex = 0;
            m_rearIndex = diff;
        }
    }

    void show() {
        log_info("cur[%u] rear[%u] max[%u]", m_curIndex, m_rearIndex, m_maxSize);
    }

private:
    uint32 m_maxSize = 0;
    uint32 m_rearIndex = 0;
    uint32 m_curIndex = 0;

    bufferType m_buffer;
};
// instantiation Dynamic Buffer
typedef Buffer<std::vector<char*>> DynamicBuffer;
template<> DynamicBuffer::Buffer();
template<> void DynamicBuffer::reserve(uint32 size);
// instantiation static buffer
typedef Buffer<char[STATIC_BUFF_MAX_SIZE]> StaticBuffer;
template<> StaticBuffer::Buffer();
template<> void StaticBuffer::reserve(uint32 size);

NS_BOOST_NETWORK_END
#endif
