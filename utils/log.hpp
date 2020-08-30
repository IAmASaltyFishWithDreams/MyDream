#ifndef __UTILS_LOG_HPP__
#define __UTILS_LOG_HPP__
#include "../common/macro.hpp"
NS_UTILS_BEGIN

//close all attr 0m black, 31m red, 32m green, 33m yellow, 34 blue, 35m purple, 36m deep green, 37m white
#define OUTPUT_NULL_COLOUR "\033[0m"
#define OUTPUT_TRACE_COLOUR "\033[32m"
#define OUTPUT_DEBUG_COLOUR "\033[37m"
#define OUTPUT_INFO_COLOUR "\033[33m"
#define OUTPUT_WARN_COLOUR "\033[35m"
#define OUTPUT_ERROR_COLOUR "\033[31m"

#define LOG_MAX_LENGTH 10240

enum LogLevel {
    LOG_LEVEL_TRACE = 0,
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO = 2,
    LOG_LEVEL_WARN = 3,
    LOG_LEVEL_ERROR  = 4,
};

enum LogControl {
    // log mode
    LOG_CONTROL_TO_CONSOLE = 0,
    LOG_CONTROL_TO_FILE = 1,
    //log Lv
    LOG_CONTROL_TRACE = 2,
    LOG_CONTROL_DEBUG = 4,
    LOG_CONTROL_INFO = 8,
    LOG_CONTROL_WARN = 16,
    LOG_CONTROL_ERROR = 32,
};

class Logger {

public:

    static void init();
    static void setControl(int32 control);
    static void unsetControl(int32 control);
    static void output(LogControl level, const char* file, int32 line, const char* func, const char * format, ...);

public:
    static bool isOutputLevel(LogControl level);

private:
    static uint32 m_logControl;

};
NS_UTILS_END

#define log_trace(format,...) UTILS::Logger::output(LogControl::LOG_CONTROL_TRACE,__FILE__,__LINE__,__FUNCTION__,format,##__VA_ARGS__)
#define log_debug(format,...) UTILS::Logger::output(LogControl::LOG_CONTROL_DEBUG,__FILE__,__LINE__,__FUNCTION__,format,##__VA_ARGS__)
#define log_info(format,...) UTILS::Logger::output(LogControl::LOG_CONTROL_INFO,__FILE__,__LINE__,__FUNCTION__,format,##__VA_ARGS__)
#define log_warn(format,...) UTILS::Logger::output(LogControl::LOG_CONTROL_WARN,__FILE__,__LINE__,__FUNCTION__,format,##__VA_ARGS__)
#define log_error(format,...) UTILS::Logger::output(LogControl::LOG_CONTROL_ERROR,__FILE__,__LINE__,__FUNCTION__,format,##__VA_ARGS__)
#define CHECK_ERROR_LOG(error,format,...)\
    if(error)\
    log_error(format,...)

#endif
