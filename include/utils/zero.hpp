
#ifndef UTILS_ZERO_HPP
#define UTILS_ZERO_HPP

#include <utils/type_traits.hpp>
#include <string>

namespace utils {

    template<class T, typename = void>
    class zero;

    template<class ScalarT>
    struct zero<ScalarT, ::std::enable_if_t<::std::is_scalar<ScalarT>::value> > {
        using type = ScalarT;

        static constexpr type value = 0;
    };

    template<class CharT, class TraitsT, class AllocatorT>
    struct zero<::std::basic_string<CharT, TraitsT, AllocatorT>, void> {
        using type = std::basic_string<CharT, TraitsT, AllocatorT>;

        static const type value;
    };

    template<class CharT, class TraitsT, class AllocatorT>
    const std::basic_string<CharT, TraitsT, AllocatorT>
            zero<std::basic_string<CharT, TraitsT, AllocatorT>, void>::value = "";

} // namespace utils

#endif //UTILS_ZERO_HPP

