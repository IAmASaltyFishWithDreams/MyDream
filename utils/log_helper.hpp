#ifndef __UTILS_LOGHELPER_HPP__
#define __UTILS_LOGHELPER_HPP__
#include "string"

#include "../common/macro.hpp"
#include "time_helper.hpp"

NS_UTILS_BEGIN

struct FuncTrace {
    FuncTrace(const char* file, const char* func, int32 line):
    m_file(file),m_func(func),m_line(line) {
        Logger::output(LOG_LEVEL_TRACE,m_file.c_str(),m_line,m_func.c_str(),"--------------------[%s]Trace Start------------", m_func.c_str());
    }
    ~FuncTrace() {
        Logger::output(LOG_LEVEL_TEACE, m_file.c_str(),m_line,m_func.c_str(), "---------------------[%s]Trace run(%d)---------", m_func.c_str(), sw.durationUs());
    }
    StopWatch sw;
    std::string m_file;
    std::string m_func;
    int32 m_line;
};

NS_UTILS_END

#define FUNC_TRACE \
        UTILS::FuncTrace ft(__FILE__,__FUNCTION__,__LINE__);
#endif
