#ifndef __UTILS_DB_CONNECTION_POOL_HPP__
#define __UTILS_DB_CONNECTION_POOL_HPP__

#include <atomic>
#include <functional>
#include <string>
#include <vector>

#include "db_base.hpp"
NS_UTILS_BEGIN

class DBObserverBase;

class DBConnectionPool {
    friend class DBObserverBase;
public:
    DBConnectionPool();
    bool init(DB_KIND kind, uint32 connMaxCount, uint32 initCount, uint32 timeOut, const std::string& host, const std::string& db, const std::string& user, const std::string& password);

    void exec(std::string& error, const char* format, ...);
    //执行select查询操作(select 等有结果集的操作)
    void execQuery(std::string& error, std::function<void(ResultSet_T result)> func, const char* format, ...);
    //执行查询语句(mysql预处理语句,select等有结果集的操作)
    void execQuery(std::string& error, std::function<void(ResultSet_T result)> func, std::vector<std::pair<uint32, std::string>>& parmVec, const char* format, ...);

    void execFunc(std::string& error, std::function<void(Connection_T conn)>func);

    void close();

private:
    URL_T m_url;
    ConnectionPool_T m_pool;
};

class DBObserverBase {
public:
    virtual void look(DBConnectionPool& pool) = 0;

    uint32 getSize(DBConnectionPool& pool) { return ConnectionPool_Size(pool.m_pool); }

    uint32 getActive(DBConnectionPool& pool) { return ConnectionPool_active(pool.m_pool); }
};

NS_UTILS_END
#endif
