
#ifndef UTILS_FINALLY_GUARD_H
#define UTILS_FINALLY_GUARD_H

#include <utils/type_traits.hpp>
#include <functional>
#include <utility>

namespace utils {

    class finally_guard {
        std::function<void()> finally_block_;
        bool active_;

    public:
        explicit inline
        finally_guard(std::function<void()> finally_block)
                : finally_block_(std::move(finally_block)), active_(true) {
        }

        finally_guard(const finally_guard &) = delete;

        inline
        finally_guard(finally_guard &&other) noexcept
                : finally_block_(std::move(other.finally_block_)), active_(other.active_) {
            other.active_ = false;
        }

        finally_guard &operator=(const finally_guard &) = delete;

        inline
        finally_guard &operator=(finally_guard &&other) noexcept {
            if (&other != this) {
                std::swap(finally_block_, other.finally_block_);
                std::swap(active_, other.active_);
            }
            return *this;
        }

        ~ finally_guard() {
            consume();
        }

        inline
        void consume() {
            if (active_) {
                active_ = false;
                finally_block_();
            }
        }
    };

    class finally_safe_guard {
        finally_guard guard_;

    public:
        explicit inline
        finally_safe_guard(finally_guard &&guard)
                : guard_(std::move(guard)) {
        }

        finally_safe_guard(const finally_safe_guard &) = delete;

        finally_safe_guard(finally_safe_guard &&) noexcept = default;

        finally_safe_guard &operator=(const finally_safe_guard &) = delete;

        finally_safe_guard &operator=(finally_safe_guard &&) noexcept = default;

        ~ finally_safe_guard() noexcept {
            consume();
        }

        inline
        void consume() noexcept {
            try {
                guard_.consume();
            }
            catch (...) {
                // catch everything for the exception safe contract
            }
        }
    };

    inline
    finally_guard make_finally(std::function<void()> &&functor) {
        return finally_guard(std::move(functor));
    }

    inline
    finally_guard make_finally_guard(std::function<void()> &&functor) {
        return make_finally(std::forward<std::function<void()>>(functor));
    }

    inline
    finally_safe_guard make_finally_safe(std::function<void()> functor) {
        return finally_safe_guard(make_finally(std::forward<std::function<void()>>(functor)));
    }

}

#endif //UTILS_FINALLY_GUARD_H
