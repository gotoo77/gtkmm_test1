
#ifndef UTILS_IGNORE_UNUSED_HPP
#define UTILS_IGNORE_UNUSED_HPP

// in case boost/core/ignore_unused is not suitable

namespace utils {

    template<typename ...Ts>
    constexpr void ignore_unused(const Ts &...) noexcept {
    }

    constexpr void ignore_unused() noexcept {
    }

} // namespace utils

#endif //UTILS_IGNORE_UNUSED_HPP
