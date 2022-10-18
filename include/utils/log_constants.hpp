#ifndef UTILS_LOG_CONSTANTS_HPP
#define UTILS_LOG_CONSTANTS_HPP

#include <syslog.h>
#include <string>

namespace utils {

    enum class log_level : int {
        none = 0,
        error = LOG_ERR,
        warning = LOG_WARNING,
        notice = LOG_NOTICE,
        info = LOG_INFO,
        debug = LOG_DEBUG
    };

    enum class syslog_facility : int {
        none = 0,
        user = LOG_USER, // NOLINT signed integer with bitwise operations
        local_0 = LOG_LOCAL0, // NOLINT signed integer with bitwise operations
        local_1 = LOG_LOCAL1, // NOLINT signed integer with bitwise operations
        local_2 = LOG_LOCAL2, // NOLINT signed integer with bitwise operations
        local_3 = LOG_LOCAL3, // NOLINT signed integer with bitwise operations
        local_4 = LOG_LOCAL4, // NOLINT signed integer with bitwise operations
        local_5 = LOG_LOCAL5, // NOLINT signed integer with bitwise operations
        local_6 = LOG_LOCAL6, // NOLINT signed integer with bitwise operations
        local_7 = LOG_LOCAL7 // NOLINT signed integer with bitwise operations
    };

} // namespace utils

inline constexpr
bool operator<=(utils::log_level l1, utils::log_level l2) {
    return static_cast<int>(l1) >= static_cast<int>(l2);
}

std::string to_string(utils::log_level value);

std::string to_string(utils::syslog_facility value);

inline
const char *to_string_3(utils::log_level level) {
    switch (level) {
        case utils::log_level::error:
            return "ERR";
        case utils::log_level::warning:
            return "WRN";
        case utils::log_level::notice:
            return "NOT";
        case utils::log_level::info:
            return "INF";
        case utils::log_level::debug:
            return "DBG";
        default:
            return "N/A";
    }
}

constexpr int logmask_up_to(utils::log_level lvl) {
    auto priority = static_cast<int>(lvl);
    // 2^(n+1) - 1
    return (1 << (priority + 1)) - 1;
}

constexpr auto syslog_facility(utils::syslog_facility fac) {
    return static_cast<int>(fac);
}

#endif //UTILS_LOG_CONSTANTS_HPP
