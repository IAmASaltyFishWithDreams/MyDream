#include "db_manager.hpp"

#include "log.hpp"
#include "sql.hpp"

NS_UTILS_BEGIN

DBManager::DBManager()
    : m_scheduler(SCHEDULE_LF_MODE_TASK_LIST) {
}

bool DBManager::init(uint32 threadCount, const DBInitData &db, const std::vector<DBTableData> &tableVec) {
    bool ret = true;
    if (threadCount < DB_MANAGER_THREAD_MIN_COUNT) {
        threadCount = DB_MANAGER_THREAD_MIN_COUNT;
    }
    ret = m_dbPool.init(db.dbKind, db.connMaxCount, db.connInitcount, db.user, db.password);
    m_threadPool.createThread([this](){ m_scheduler.run(); }, threadCount);
    m_scheduler.start();
    ret = loadTableField(tableVec);
    return ret;
}

void DBManager::stop() {
    m_scheduler.stop();
    m_threadPool.join();
}

bool DBManager::loadTableField(const std::vector<DBTableData>& tableVec) {
    bool ret = true;

    auto dealResult = [this,&ret](const std::string& table,ResultSet_T result){
        DBTableCache tableCache;
        std::vector<SQLField> fieldVec;
        tableCache.table = table;
        while(ResultSet_next(result)){
            std::string& fieldName = ResultSet_getString(result, 1);
            std::string& fieldTypeStr = ResultSet_getString(result, 2);
            uint32 fieldType = DATA_TYPE_UNKNOWN;
            if (fieldTypeStr.find("bigint") != std::string::npos) {
                if (fieldTypeStr.find("unsigned") != std::string::npos) {
                    fieldType = DATA_TYPE_ULLONG;
                } else {
                    fieldType = DATA_TYPE_LLONG;
                }
            } else if (fieldTypeStr.find("int") != std::string::npos) {
                if (fieldTypeStr.find("unsigned") != std::string::npos) {
                    fieldType = DATA_TYPE_UINT;
                } else {
                    fieldType = DATA_TYPE_INT;
                }
            } else if (fieldTypeStr.find("blob") != std::string::npos) {
                fieldType = DATA_TYPE_BLOB;
            } else if (fieldTypeStr.find("varchar") != std::string::npos || fieldTypeStr.find("text") != std::string::npos) {
                fieldType = DATA_TYPE_STRING;
            } else {
                //TODO other type not deal with
            }

            if (fieldType == DATA_TYPE_UNKNOWN) {
                ret = false;
                log_error("sql table [%s] field [%s] type [%s] failed", table.c_str(), fieldName.c_str(), fieldTypeStr.c_str());
                return ret;
            }
            fieldVec.push_back({fieldType, fieldName});
        }
        tableCache.sqlPtr = std::make_shared<SQLField>(table,tableCache.where, fieldVec);
        m_tableCacheMap[table] = tableCache;
    };

    std::string error;
    for(const auto& value : tableVec) {
        m_dbPool.exeQuery(error, std::bind(dealResult, value.table, std::placeholders::_1), "SHOW COLUMOS FROM [%s]", value.table.c_str());
        if (error != DB_EXEC_SUCCEED) {
            log_error("db exec error : %s", error.c_str());
            return false;
        }
        if (!ret) {
            return ret;
        }
        m_tableCacheMap[value.table].where = value.where;
    }
    return ret;
}

SqlTablePtr DBManager::getTableSql(const char* table){
    if (table == NULL) {
        log_error("table == NULL");
            return NULL;
    }
    auto it = m_tableCacheMap.find(table);
    if (it == m_tableCacheMap.end()) {
        log_error("not find table [%s]", table);
        return NULL;
    }
    return it->second.sqlPtr;
}

NS_UTILS_END
