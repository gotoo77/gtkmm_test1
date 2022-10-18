
#include "utils/log_constants.hpp"

std::string to_string(utils::log_level value) {
    switch (value) {
        case utils::log_level::error:
            return "ERROR";
        case utils::log_level::warning:
            return "WARNING";
        case utils::log_level::notice:
            return "NOTICE";
        case utils::log_level::info:
            return "INFO";
        case utils::log_level::debug:
            return "DEBUG";
        default:
            return "<none>";
    }
}

std::string to_string(utils::syslog_facility value) {
    switch (value) {
        case utils::syslog_facility::user:
            return "USER";
        case utils::syslog_facility::local_0:
            return "LOCAL_0";
        case utils::syslog_facility::local_1:
            return "LOCAL_1";
        case utils::syslog_facility::local_2:
            return "LOCAL_2";
        case utils::syslog_facility::local_3:
            return "LOCAL_3";
        case utils::syslog_facility::local_4:
            return "LOCAL_4";
        case utils::syslog_facility::local_5:
            return "LOCAL_5";
        case utils::syslog_facility::local_6:
            return "LOCAL_6";
        case utils::syslog_facility::local_7:
            return "LOCAL_7";
        default:
            return "<none>";
    }
}