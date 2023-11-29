
#ifndef UTILS_GLOBAL_H
#define UTILS_GLOBAL_H

#include <utils/log_constants.hpp>

constexpr std::size_t USER_LEN = 64 + 1;

typedef struct ConfigSyslog {
    utils::log_level level = utils::log_level::none;
    utils::syslog_facility facility = utils::syslog_facility::none;
    bool console = false;
} rConfigSyslog;

struct Globals {
    rConfigSyslog cfg_syslog;    /*!< structure de config syslog */
    bool verbose{};              /*!< extra verbose log */
    char Server1[USER_LEN]{};    /*!< hostname/IP du serveur -tentative 1- pour connexion a la BDD (SQL) */
    bool test_flag{};            /*!<   */

};

#endif // UTILS_GLOBAL_H
