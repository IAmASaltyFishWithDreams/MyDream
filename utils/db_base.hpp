#ifndef __UTILS_DB_BASE_HPP__
#define __UTILS_DB_BASE_HPP__

#include <map>
#include <string>
#include <memory>
#include <vector>
#include <zdb.h>

#include "macro.hpp"

NS_UTILS_BEGIN

#define DB_POOL_MIN_COUNT 5

#define DB_CONN_MIN_TIMEOUT 10

#define DB_ERROR_LINE 10000

#define SQL_MAX_LINE 5000

#define DB_EXEC_SUCCEED "db exec succeed"
#define DB_CONN_IS_NULL "db connect is null"

#define DB_MANAGER_THREAD_MIN_COUNT 1


enum DB_KIND {
    DB_KIND_MYSQL  = 1,
    DB_KIND_POSTGRESQL = 2,
    DB_KIND_SQLITE = 3,
    DB_KIND_ORACLE = 4,
};

enum DATA_TYPE {
    DATA_TYPE_UNKNOWN = 0,
    DATA_TYPE_STRING  = 1,
    DATA_TYPE_INT = 2,
    DATA_TYPE_LLONG = 3,
    DATA_TYPE_UINT = 4,
    DATA_TYPE_ULLONG = 5,
    DATA_TYPE_DOUBLE = 6,
    DATA_TYPE_BLOB = 7,
    DATA_TYPE_TIMESTRMP = 8,
};

const char* getTypeFormatParam(uint32_t type);

struct DBInitData {
    DB_KIND dbKind;
    uint32 connMaxCount;
    uint32 connInitcount;
    uint32 timeOut;

    std::string host;
    std::string database;
    std::string user;
    std::string password;
};

struct DBTableData {
    std::string table;
    std::string where;
};

NS_UTILS_END
#endif
