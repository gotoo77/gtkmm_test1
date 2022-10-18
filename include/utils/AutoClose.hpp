
#ifndef UTILS_AUTOCLOSE_HPP
#define UTILS_AUTOCLOSE_HPP

#include <utils/type_traits.hpp>
#include <utils/finally_guard.hpp>
#include <vector>
#include <functional>

namespace utils {

/**
 * Stack up global initializers so that is not necessary to explicitly call the global
 * releases.
 */
    class AutoClose {
        std::vector<utils::finally_safe_guard> handlers_;

    public:
        using closefn_type = std::function<void()>;

        AutoClose &acquireClose(closefn_type &&close) {
            handlers_.emplace(handlers_.begin(), utils::make_finally_safe(std::move(close)));
            return *this;
        }

        /**
         * @brief Acquire a global release
         *
         * Calls the initialization function with the provided arguments and store
         * the release function returned by the initialization.
         *
         * @tparam Args the argument types of the initialization function
         * @tparam InitFn the type of the initialization function
         *
         * @param init the initialization function
         * @param args the initialization arguments
         * @return this AutoClose
         */
        template<class ...Args, typename InitFn = closefn_type (*)(Args ...)>
        inline
        AutoClose &acquire(InitFn init, Args &&... args) {
            using initfn_return_type = decltype(std::declval<InitFn>()(std::forward<Args>(args)...));
            static_assert(std::is_convertible<initfn_return_type, closefn_type>::value,
                          "init must return the close function with type <void()>");

            closefn_type close = init(std::forward<Args>(args)...);
            return acquireClose(std::move(close));
        }

        /**
         * @brief Close all acquired release functions
         *
         * closeAll may be called explicitly, but not required as acquired guards are consumed during destruction
         */
        inline
        void closeAll() noexcept {
            handlers_.clear();
        }
    };

} // namespace utils

#endif //UTILS_AUTOCLOSE_HPP
