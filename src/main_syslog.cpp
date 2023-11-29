
#include <unistd.h>
#include <iostream>
#include <map>

#include "string_gen.hpp"
#include "myAppInfo.hpp"
#include "cond_print.hpp"

#ifdef _WIN32
#include <win_ports/syslog.h>
#else
#include <syslog.h>
#endif


int demo_syslog(){
    init_syslog("localhost");
    myAppInfo prog("main_syslog", "0.1.0");

    constexpr unsigned int ONE_MS = 1000;
    constexpr unsigned int tempo_ums_1 = 250* ONE_MS;
    constexpr unsigned int tempo_ums_2 = 1000 *ONE_MS;

    WARN_IF( 2 == 6/3);

    std::map<int,std::string> log_facilities = {
            {LOG_LOCAL0,STR(LOG_LOCAL0)},
            {LOG_LOCAL1,STR(LOG_LOCAL1)},
            {LOG_LOCAL2,STR(LOG_LOCAL2)},
            {LOG_LOCAL3,STR(LOG_LOCAL3)},
            {LOG_LOCAL4,STR(LOG_LOCAL4)},
            {LOG_LOCAL5,STR(LOG_LOCAL5)},
            {LOG_LOCAL6,STR(LOG_LOCAL6)},
            {LOG_LOCAL7,STR(LOG_LOCAL7)},
            {LOG_USER  ,STR(LOG_USER)},
    };

    std::map<int,std::string> log_levels = {
            {LOG_EMERG,STR(LOG_EMERG)},
            {LOG_ALERT,STR(LOG_ALERT)},
            {LOG_CRIT ,STR(LOG_CRIT)},
            {LOG_ERR  ,STR(LOG_ERR)},
            {LOG_WARNING,STR(LOG_WARNING)},
            {LOG_NOTICE,STR(LOG_NOTICE)},
            {LOG_EMERG,STR(LOG_EMERG)},
            {LOG_INFO,STR(LOG_INFO)},
            {LOG_DEBUG,STR(LOG_DEBUG)},
    };

    std::string to_log;

    for (auto & fac: log_facilities) {
        std::cout << ">using log facility " << fac.second << std::endl;

        openlog(prog.getFullName().c_str(), LOG_PID, fac.first);
        setlogmask (LOG_UPTO (LOG_DEBUG));

        for (auto &level:log_levels) {
            auto s1 = random_string(13);
            auto s2 = generate_random_alphanumeric_string(5);
            to_log = std::string(__FUNCTION__) + " ";
            to_log.append(level.second).append(" : ").append(s1).append(" - ").append(s2);
            std::cout << to_log << std::endl;
            syslog(level.first, to_log.c_str());
            usleep(tempo_ums_1);
        }
        std::cout << ">>changing log facility" << std::endl;
        usleep(tempo_ums_2);
        closelog();
    }

    return EXIT_SUCCESS;
}

int main (int argc, char *argv[])
{
    return demo_syslog();
}