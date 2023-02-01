
#include <utils/RestClientInstance.hpp>
#include <restclient-cpp/restclient.h>
#include "include/utils/log/MyLog.hpp"

namespace utils {

    static std::weak_ptr<RestClientInstance> _RestClientSharedReference;
    static std::shared_ptr<RestClientInstance> _RestClient;

    RestClientInstance::RestClientInstance() {
        RestClient::init();
    }

    RestClientInstance::~RestClientInstance() {
        RestClient::disable();
    }

    std::shared_ptr<RestClientInstance> RestClientInstance::getInstance() {
        auto instance = _RestClientSharedReference.lock();
        if (instance == nullptr) {
            instance = std::shared_ptr<RestClientInstance>{new RestClientInstance()};
            _RestClientSharedReference = instance;
        }
        return instance;
    }

    std::function<void()> RestClientInstance::init() {
        if (_RestClient == nullptr) {
            LOG_I("RestClientInstance initialize");
            _RestClient = getInstance();
            return &RestClientInstance::disable;
        }
        return {};
    }

    void RestClientInstance::disable() {
        if (_RestClient) {
            LOG_I("RestClientInstance disable");
            _RestClient = nullptr;
        }
    }

}
