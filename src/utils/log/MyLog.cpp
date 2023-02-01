/*!
 * @file src/utils/MyLog.cpp
 * @brief logging methods/utilities
*/

#include "include/utils/log/MyLog.hpp"
#include <sstream>
#include <iomanip>

static constexpr int PBXLOG_SPACE = 40;

utils::log_level Log::log_level_ = utils::log_level::none;

std::function<void()> Log::init(Init &&init) {
    Init localInit = std::move(init); //NOLINT this is required by sonar
    if (log_level_ == utils::log_level::none
        && localInit.level != utils::log_level::none && localInit.facility != utils::syslog_facility::none) {
        setlogmask(LOG_UPTO(static_cast<int>(localInit.level))); // NOLINT signed int with bitwise operation
#ifdef NDEBUG
        openlog(localInit.program, LOG_PID, static_cast<int>(localInit.facility));
#else
#pragma message "Log compiled with debug console switch"
        openlog(localInit.program,
                (LOG_PID | (localInit.console ? LOG_PERROR : 0U)), // NOLINT
                static_cast<int>(localInit.facility));
#endif
        log_level_ = localInit.level;
        return Log::close;
    }
    return {};
}

void Log::close() {
    closelog();
    log_level_ = utils::log_level::none;
}

std::string Log::computeTagFormat(utils::log_level level, const char *function, const char *format) {
    std::string tag = std::string("[") + function + "] ";
    std::ostringstream oss;
    oss << std::string("(") + to_string_3(level) + ") ";
    oss << std::setfill(' ') << std::setw(PBXLOG_SPACE) << std::right << tag;
    oss << format;
    return oss.str();
}

std::string Log::computeMethodName(const std::string &function, const std::string &prettyFunction) {
    // If the input is a constructor, it gets the beginning of the class name, not of the method.
    // That's why later on we have to search for the first parenthesis
    std::size_t locFunName = prettyFunction.find(function);
    std::size_t begin = prettyFunction.rfind(' ', locFunName) + 1;
    // Adding function.length() make this faster and also allows to handle operator parenthesis!
    std::size_t end = prettyFunction.find('(', locFunName + function.length());
    return (prettyFunction.substr(begin, end - begin));

}
