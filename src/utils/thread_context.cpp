
#include "utils/thread_context.hpp"
#include <atomic>

namespace utils {

    thread_local unsigned thread_context::id_ = 0;

    void thread_context::init() noexcept {
        static std::atomic_uint id_provider;
        id_ = id_provider++;
    }

} // namespace utils