
#ifndef UTILS_THREAD_CONTEXT_HPP
#define UTILS_THREAD_CONTEXT_HPP

namespace utils {

/**
 * Handles logical values of a thread context.
 */
    class thread_context final {
        static thread_local unsigned id_;

    public:
        /**
         * @brief Initializes all logical values of the thread context.
         *
         * - id: uses an atomic_uint to assign the id to an incremented unique value.
         */
        static void init() noexcept;

        /**
         * @brief The logical thread id (starts at 0)
         *
         * @return Get the thread local logical id.
         */
        static unsigned id() noexcept;
    };

    inline
    unsigned thread_context::id() noexcept {
        return id_;
    }

} // namespace utils

#endif //UTILS_THREAD_CONTEXT_HPP
