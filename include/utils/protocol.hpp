
#ifndef UTILS_PROTOCOL_HPP
#define UTILS_PROTOCOL_HPP

#include <string>

namespace utils {

    enum class protocol {
        none,
        http
    };

    enum class transport {
        tcp,
        ssl_tcp
    };

    enum class ssl_method {
        none,
        tls11,
        tls12
    };

} // namespace utils

std::string to_string(utils::protocol value);

std::string to_string(utils::transport value);

std::string to_string(utils::ssl_method value);

#endif //UTILS_PROTOCOL_HPP
