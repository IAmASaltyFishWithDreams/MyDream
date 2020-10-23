#include "sql.hpp"

#include <sstream>

#include "db_base.hpp"

NS_UTILS_BEGIN

SqlTable::SqlTable(const std::string& table, const std::string& where, const std::vector<SQLField>& fieldVec)
    : m_table(table),m_where(where),m_fieldVec(fieldVec) {
        std::ostringstream oss;
        //m _selectAll
        oss << "select ";
        for (uint32 i = 0; i < fieldVec.size(); i++){
            if (i > 0) {
                oss << ", ";
            }
            oss << fieldVec[i].name;
        }
        oss << "from " << m_table << ";";
        m_selectAll = oss.str();
        oss.str("");
        //m_seleLimit
        oss<< "select ";
        for(uint32 i = 0; i < fieldVec.size(); i++) {
            if(i > 0) {
                oss << ", ";
            }
            oss << fieldVec[i].name;
        }
        oss << "from " << m_table << " limit %u, %u;";
        m_selectLimit = oss.str();
        oss.str("");

        //m_selectWhere
        oss<< "select ";
        for(uint32 i = 0; i < fieldVec.size(); i++) {
            if(i > 0) {
                oss << ", ";
            }
            oss<< fieldVec[i].name;
        }
        oss << "from " << m_table << "where " << m_where << " ;";
        m_selectWhere = oss.str();
        oss.str("");
        //m_selectCount
        oss << "select count(*) as num from " << m_table << " ;";
        m_selectCount = oss.str();
        oss.str("");

        oss << "update " << m_table << " set ";

        for(uint32 i = 0; i < fieldVec.size(); i++) {
            if (i > 0) {
                oss << ", ";
            }
            oss << "set " << fieldVec[i].name << " = " << getTypeFormatParam(fieldVec[i].type);
        }
        oss << "where " << m_where << ";";
        m_update = oss.str();
        oss.str("");

        //insert
        oss<< " insert into " << m_table << "(";
        for(uint32 i = 0; i < fieldVec.size(); i++) {
            if(i > 0) {
                oss << ", ";
            }
            oss << fieldVec[i].name;
        }
        oss << " ) value (";
        for(uint32 i = 0; i < fieldVec.size(); i++) {
            if (i>0) {
                oss << ", ";
            }
            oss << getTypeFormatParam(fieldVec[i].type);
        }
        oss << ");";
        m_insert = oss.str();
        oss.str("");
    }

const std::string& SqlTable::getSelectAll() {
    return m_selectAll;
}
const std::string& SqlTable::getSelectLimit() {
    return m_selectLimit;
}
const std::string& SqlTable::getSelectCount() {
    return m_selectCount;
}
const std::string& SqlTable::getSelectWhere() {
    return m_selectWhere;
}
const std::string& SqlTable::getUpdate() {
    return m_update;
}
const std::string& SqlTable::getInsert() {
    return m_insert;
}

NS_UTILS_END
