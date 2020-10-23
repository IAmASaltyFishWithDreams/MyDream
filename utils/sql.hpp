#ifndef __UTILS_SQL_HPP__
#define __UTILS_SQL_HPP__

#include <memory>
#include <string>
#include <vector>

#include "macro.hpp"

NS_UTILS_BEGIN

struct SQLField {
    uint32 type;
    std::string name;
};

class SqlTable {
public:
    SqlTable(const std::string& table, const std::string& where, const std::vector<SQLField>& fieldVec);
    // get all
    const std::string& getSelectAll();
    //select Limit
    const std::string& getSelectLimit();
    //select count
    const std::string& getSelectCount();
    //seletwhere
    const std::string& getSelectWhere();
    //update
    const std::string& getUpdate();
    //inster
    const std::string& getInsert();

private:
    std::string m_table;
    std::string m_where;
    std::vector<SQLField> m_fieldVec;

    std::string m_selectAll;
    std::string m_selectLimit;
    std::string m_selectCount;
    std::string m_selectWhere;
    std::string m_update;
    std::string m_insert;

};

typedef std::shared_ptr<SqlTable> SqlTablePtr;

NS_UTILS_END
#endif
