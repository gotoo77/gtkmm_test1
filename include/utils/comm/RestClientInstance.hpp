
#ifndef UTILS_RESTCLIENTINSTANCE_HPP
#define UTILS_RESTCLIENTINSTANCE_HPP

#include <memory>
#include <functional>

namespace utils {

/**
 * Init guard for lib-restclientcpp
 */
    class RestClientInstance //NOSONAR rule of five here
    {
        RestClientInstance();

    public:
        static std::shared_ptr<RestClientInstance> getInstance();

        static std::function<void()> init();

        static void disable();

        ~ RestClientInstance();

        RestClientInstance(const RestClientInstance &) = delete;

        RestClientInstance(RestClientInstance &&) = delete;

        RestClientInstance &operator=(const RestClientInstance &) = delete;

        RestClientInstance &operator=(RestClientInstance &&) = delete;
    };

} //namespace utils

#endif //UTILS_RESTCLIENTINSTANCE_HPP
