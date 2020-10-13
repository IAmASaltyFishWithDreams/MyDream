#ifndef __BOOST_NETWORK_COMMON_HPP__
#define __BOOST_NETWORK_COMMON_HPP__
#include "../utils/macro.hpp"

#pragma pack(1)
// memory sort 1
struct MsgHead {
    uint16 cmd;
    uint16 len;
    uint8 index;
    uint8 flag;
};

#pragma pack()

enum PACK_FLAG {
    PACK_FLAG_ENCYPT = 0x01,    //encypt
    PACK_FLAG_ZIP = 0x02,       //zip
    PACK_FLAG_UID = 0x04,       //&uid
};

//notes  dynamic buff base size
#define DYNAMIC_BUFF_BASE_SIZE (64*1024)
// buff formula
#define DYNAMIC_BUFF_EXPAND_TIMES(size) (size + DYNAMIC_BUFF_BASE_SIZE - 1) / DYNAMIC_BUFF_BASE_SIZE
// static buff size
#define STATIC_BUFF_MAX_SIZE (64* 10 * 1024)

//pack max size 65535
static const uint32 PACKET_MAX_SIZE = 64 * 1024 -1;
//pack head size
static const uint32 PACKET_HEAD_SIZE = sizeof(MsgHead);
//pack body size
static const uint32 PACKET_BODY_MAX_SIZE = PACKET_MAX_SIZE - PACKET_HEAD_SIZE;

//compress min
static const uint32 PACKET_COMPRESS_MIN_SIZE = 128;
// compress 1M = 60K not more than this
static const uint32 PACKET_UNCOMPRESS_MAX_SIZE = 1024 *1024;

#endif
