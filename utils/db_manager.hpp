#ifndef __UTILS_DB_MANAGER_HPP__
#define __UTILS_DB_MANAGER_HPP__

#include <vector>
#include <map>
#include "db_base.hpp"
#include "db_connection_pool.hpp"
#include "scheduler_leader_follower.hpp"
#include "sql.hpp"
#include "macro.hpp"
#include "thread_pool.hpp"

NS_UTILS_BEGIN

struct DBTableCache {
    std::string table;
    std::string where;
    SqlTablePtr sqlPtr;
};

class DBManager {
public:
    DBManager();
    ~DBManager(){};

    bool init(uint32 threadCount, DBInitData& db, const std::vector<DBTableData>& tableVec);

    void stop();

    // submit
    void posDBTask(UnitPtr& ptr) { m_scheduler.post(ptr); }

public:
    SqlTablePtr getTableSql(const char* table );
    DBConnectionPool& getDBConnPool() { return m_dbPool; }
    ThreadPool& getScheduler() {return m_scheduler; }

private:
    bool loadTableField(const std::vector<DBTableData>& tableVec);

private:
    DBConnectionPool m_dbPool;  //数据库链接池
    SchedulerLF m_scheduler;    //调度器(消息队列模式)
    ThreadPool m_threadPool;    //线程池
private:
    std::map<std::string, DBTableCache> m_tableCacheMap;
};


NS_UTILS_END

#endif
