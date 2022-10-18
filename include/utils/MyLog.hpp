/*!
 * @file include/MyLog.hpp
 * @brief header file for logging methods/utilities
 * @author gduval
 * @date on 24/01/22
*/

#ifndef UTILS_MYLOG_HPP
#define UTILS_MYLOG_HPP

#include <cinttypes> //for PRIu64

#include <functional>
#include <utils/log_constants.hpp>
#ifdef _WIN64
#include <win_ports/syslog.h>
#else
#include <syslog.h>
#endif

class Log {
    static utils::log_level log_level_;

public:
    struct Init {
        const char *program;
        utils::log_level level;
        utils::syslog_facility facility;
        bool console;
    };

    /**
     * Initialize syslog. The returned close function is compatible with AutoClose::acquire.
     *
     * @param level the level filter
     * @param facility the syslog facility
     * @return the close function
     */
    static std::function<void()> init(Init &&init);

private:
    static void close();

public:
    template<utils::log_level level, typename ... Args>
    static void log(const char *function, const char *format, Args &&... args);

    template<typename ... Args>
    inline static
    void error(const char *function, const char *format, Args &&... args) {
        log<utils::log_level::error>(function, format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    inline static
    void warning(const char *function, const char *format, Args &&... args) {
        log<utils::log_level::warning>(function, format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    inline static
    void notice(const char *function, const char *format, Args &&... args) {
        log<utils::log_level::notice>(function, format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    inline static
    void info(const char *function, const char *format, Args &&... args) {
        log<utils::log_level::info>(function, format, std::forward<Args>(args)...);
    }

    template<typename ... Args>
    inline static
    void debug(const char *function, const char *format, Args &&... args) {
        log<utils::log_level::debug>(function, format, std::forward<Args>(args)...);
    }

    inline static
    bool is_error_active() {
        return log_level_ <= utils::log_level::error;
    }

    inline static
    bool is_warning_active() {
        return log_level_ <= utils::log_level::warning;
    }

    inline static
    bool is_notice_active() {
        return log_level_ <= utils::log_level::notice;
    }

    inline static
    bool is_info_active() {
        return log_level_ <= utils::log_level::info;
    }

    inline static
    bool is_debug_active() {
        return log_level_ <= utils::log_level::debug;
    }

    static std::string computeTagFormat(utils::log_level level, const char *function, const char *format);

    static std::string computeMethodName(const std::string &function, const std::string &prettyFunction);
};

#define __COMPACT_PRETTY_FUNCTION__ Log::computeMethodName(__FUNCTION__,__PRETTY_FUNCTION__).c_str()

#define LOG_E(Mask, ...)   LLOG_E(__COMPACT_PRETTY_FUNCTION__, Mask, ## __VA_ARGS__)
#define LOG_W(Mask, ...)   LLOG_W(__COMPACT_PRETTY_FUNCTION__, Mask, ## __VA_ARGS__)
#define LOG_N(Mask, ...)   LLOG_N(__COMPACT_PRETTY_FUNCTION__, Mask, ## __VA_ARGS__)
#define LOG_I(Mask, ...)   LLOG_I(__COMPACT_PRETTY_FUNCTION__, Mask, ## __VA_ARGS__)
#define LOG_D(Mask, ...)   LLOG_D(__COMPACT_PRETTY_FUNCTION__, Mask, ## __VA_ARGS__)

// logs __FUNCTION__ in lambda expands as operator(); better use a custom set value
#define LLOG_E(func, format, ...)  Log::error(func, format, ## __VA_ARGS__)
#define LLOG_W(func, format, ...)  Log::warning(func, format, ## __VA_ARGS__)
#define LLOG_N(func, format, ...)  Log::notice(func, format, ## __VA_ARGS__)
#define LLOG_I(func, format, ...)  Log::info(func, format, ## __VA_ARGS__)
#define LLOG_D(func, format, ...)  Log::debug(func, format, ## __VA_ARGS__)


template<utils::log_level level, typename ... Args>
inline
void Log::log(const char *function, const char *format, Args &&... args) {
    static constexpr utils::log_level lvl = level; //NOSONAR cpp:LiteralSuffix not a literal suffix
    std::string tag = computeTagFormat(lvl, function, format);
    syslog(static_cast<int>(lvl), tag.c_str(), std::forward<Args>(args)...);
}

#endif //UTILS_MYLOG_HPP
