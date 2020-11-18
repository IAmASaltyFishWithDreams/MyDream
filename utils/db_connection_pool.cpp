#include "db_connection_pool.hpp"
#include <stdarg.h>

#include "calc_tool.hpp"
#include "log.hpp"

NS_UTILS_BEGIN

DBConnectionPool::DBConnectionPool() : m_url(NULL), m_pool(NULL){
}

bool DBConnectionPool::init(DB_KIND kind, uint32 connMaxCount, uint32 initCount, uint32 timeOut, const std::string& host, const std::string& db, const std::string& user, const std::string& password) {
    std::string dbKind;
    if (kind == DB_KIND_MYSQL) {
        dbKind = "mysql";
    } else if (kind == DB_KIND_ORACLE) {
        dbKind = "oracle";
    } else if (kind == DB_KIND_SQLITE) {
        dbKind = "sqlite";
    } else if (kind == DB_KIND_POSTGRESQL) {
        dbKind = "postgresql";
    } else {
        log_error(" DB kind [%u] not exist", kind);
        return false;
    }

    char buf[256] = {0};
    snprintf(buf, sizeof(buf), "%s://%s/%s?user=%s&password=%s&charset=utf48mb4", dbKind.c_str(), host.c_str(), db.c_str(), user.c_str(), password.c_str());
    m_url = URL_new(buf);
    if (m_url == NULL) {
        log_error("m_url == NULL, buf[%s]",buf);
        return false;
    }
    m_pool = Connection_New(m_url);
    if (m_pool == NULL) {
        log_error("m_pool == NULL,");
        URL_free(&m_url);
        return false;
    }

    if (timeOut < DB_CONN_MIN_TIMEOUT) {
        timeOut = DB_CONN_MIN_TIMEOUT;
    }
    if (connMaxCount < DB_POOL_MIN_COUNT) {
        connMaxCount = DB_POOL_MIN_COUNT;
    }
    if (initCount > connMaxCount) {
        initCount = connMaxCount;
    }

    ConnectionPool_setInitialConnections(m_pool, initCount);
    ConnectionPool_setMaxConnections(m_pool, connMaxCount);
    ConnectionPool_setConnectionTimeout(m_pool, timeOut);
    ConnectionPool_setReaper(m_pool, initCount);
    ConnectionPool_setAbortHandler(m_pool, [](const char* error){ log_error("Connection Pool error [%s]", error); });
    ConnectionPool_start(m_pool);

    log_info("DB %s%s%s, connection pool init succeed,  init count[%u], connmax count[%u]", dbKind.c_str(), host.c_str(), db.c_str(), initCount, connMaxCount);
    return true;
}

void DBConnectionPool::exec(std::string& error, const char* format, ...) {
    Connection_T conn = ConnectionPool_getConnection(m_pool);
    if (conn == NULL) {
        error = DB_CONN_IS_NULL;
        return;
    }
    char sql[SQL_MAX_LINE] = {0};
    va_list args;
    va_start(args,format);
    std::vsnprintf(sql, SQL_MAX_LINE-1, format, args);
    va_end(args);
    TRY
    {
        Connection_exec(conn, "%s", sql);
        error = DB_EXEC_SUCCEED;
    } CATCH(SQLException) {
        char buf[DB_ERROR_LINE] = {0};
        snprintf(buf, sizeof(buf), "sql [%s] error [%s]", sql, Exception_frame.message;
        error = buf;
    }
    END_TRY
    va_end(args);
    Connection_close(conn);
}

void DBConnectionPool::execQuery(std::string& error, std::function<void(ResultSet_T result)> func, const char* format, ...) {
    Connection_T conn = ConnectionPool_getConnection(m_pool);
    if(conn == NULL) {
        error = DB_CONN_IS_NULL;
        return;
    }
    char sql[SQL_MAX_LINE] = {0};
    va_list args;
    va_start(args, format);
    std::vsnprintf(sql, SQL_MAX_LINE -1, format, args);
    va_end(args);
    TRY{
        ResultSet_T result = Connection_executeQuery(conn, "%s", sql);
        func(result);
        error = DB_EXEC_SUCCEED;
    } CATCH(SQLException) {
        char buf[DB_ERROR_LINE] = {0};
        snprintf(buf, sizeof(buf), "sql[%s] error[%s]", sql, Exception_frame.message);
        error = buf;
    }
    END_TRY
    va_end(args);
    Connection_close(conn);
}

void DBConnectionPool::execQuery(std::string& error, std::function<void(ResultSet_T result)> func, std::vector<uint32, std::string>& parmVec, const char* format, ...) {
    Connection_T conn = ConnectionPool_getConnection(m_pool);
    if(conn == NULL) {
        error = DB_CONN_IS_NULL;
        return;
    }
    char sql[SQL_MAX_LINE] = {0};
    va_list args;
    va_start(args,format);
    std::vsnprintf(sql, SQL_MAX_LINE -1, format, args);
    va_end(args);
    TRY{
        PreparedStatement_T ps = ConnectionPool_PreparedStatement(conn, "%s", sql);
        for(uint32 i = 0; i < parmVec.size(); i++) {
            switch(parmVec.at(i).first) {
                case DATA_TYPE_STRING:
                    PreparedStatement_setString(ps, i+1, parmVec.at(i).second.c_str());
                    break;
                case DATA_TYPE_INT:
                    PreparedStatement_setInt(ps, i+1, string_to_int32(parmVec.at(i).second));
                    break;
                case DATA_TYPE_LLONG:
                    PreparedStatement_setLLong(ps, i+1, string_to_int64(parmVec.at(i).second));
                    break;
                case DATA_TYPE_UINT:
                    PreparedStatement_setInt(ps, i+1, string_to_uint32(parmVec.at(i).second));
                    break;
                case DATA_TYPE_ULLONG:
                    PreparedStatement_setLLong(ps, i+1, string_to_uint64(parmVec.at(i).second));
                    break;
                case DATA_TYPE_DOUBLE:
                    //notes now not need this type
                    break;
                case DATA_TYPE_TIMESTRMP:
                    PreparedStatement_setTimeStrmp(ps, i+1, string_to_int32(parmVec.at(i).second));
                    break;
                case DATA_TYPE_BLOB:
                    PreparedStatmen_setBlob(ps, i+1, parmVec.at(i).second.c_str(), parmVec.at(i).second.size());
                    break;
            }
        }
        ResultSet_t result = Connection_executeQuery(ps);
        func(result);
        error = DB_EXEC_SUCCEED;
    } CATCH(SQLException){
        char buf[SQL_MAX_LINE] = {0};
        snprintf(buf, sizeof(buf), "sql[%s] error[%s]", sql, Execption_frame.message);
        error = buf;
    }
    END_TRY
    va_end(args);
    Connection_close(conn);
}

void DBConnectionPool::execFunc(std::string& error, std::function<void(ResultSet_T result)> func) {
    Connection_T conn = ConnectionPool_getConnection(m_pool);
    if(conn == NULL) {
        error = DB_CONN_IS_NULL;
        return;
    }
    TRY{
        func(conn);
        error = DB_EXEC_SUCCEED;
    } CATCH(SQLExecption) {
        char buf[SQL_MAX_LINE] = {0};
        snprintf(buf, sizeof(buf), "exec func error[%s]", Execption_frame.message);
        error = buf;
    }
    END_TRY
    Connection_close(conn);
}

void DBConnectionPool::close() {
    if (m_pool != NULL) {
        ConnectionPool_stop(m_pool);
        ConnectionPool_free(&m_pool);
    }
    if (m_url != NULL) {
        URL_free(&m_url);
    }
}

NS_UTILS_END
