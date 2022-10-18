
#include "utils/protocol.hpp"

std::string to_string(utils::protocol value) {
    switch (value) {
        case utils::protocol::http:
            return "HTTP";
        default:
            return "<none>";
    }
}

std::string to_string(utils::transport value) {
    switch (value) {
        case utils::transport::tcp:
            return "TCP";
        case utils::transport::ssl_tcp:
            return "SSL+TCP";
    }
    // unreachable code
    // solves warning: control reaches end of non-void function
    return "<none>";
}

std::string to_string(utils::ssl_method value) {
    switch (value) {
        case utils::ssl_method::tls11:
            return "TLS-1.1";
        case utils::ssl_method::tls12:
            return "TLS-1.2";
        default:
            return "<none>";
    }
}
