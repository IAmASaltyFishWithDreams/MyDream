#ifndef __COMMON_MACRO_HPP__
#define __COMMON_MACRO_HPP__

/**
 * 定义server的所用宏
 * abstract one Folder define rules:one folder one define
*/

#define  NS_SERVICE_CORE_BEGIN namespace SERVICE_CORE {
#define  NS_SERVICE_CORE_END }
#define  USING_NS_SERVICE_CIRE using namespace SERVICE_CORE

#define  NS_BOOST_NETWORK_BEGIN namespace BOOST_NETWORK {
#define  NS_BOOST_NETWORK_END }
#define  USING_NS_BOOST_NETWORK using namespace BOOST_NETWORK

#define  NS_UTILS_BEGIN namespace UTILS {
#define  NS_UTILS_END }
#define  USING_NS_UTILS using namespace UTILS

/* #define USING_BOOST_SERVER_BEGIN namespace BOOST_SERVER { */
/* #define USING_BOOST_SERVER_END } */
/* #define USING_SINGLE_SERVER_BEGIN namespace SINGLE_SERVER { */
/* #define USING_SINGLE_SERVER_END } */

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

#endif
