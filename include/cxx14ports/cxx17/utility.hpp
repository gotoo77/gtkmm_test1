
#ifndef CXX14PORTS_CXX17_UTILITY_HPP
#define CXX14PORTS_CXX17_UTILITY_HPP

#include <utility>
#include <cstdint>

#if (__cplusplus < 201700L) && (__cplusplus >= 201401L)

namespace cxx14ports {
    namespace cxx17 {

        struct in_place_t {
            constexpr explicit in_place_t(std::int_fast32_t) noexcept {}
        };

        constexpr in_place_t in_place{0};

        template<class T>
        struct in_place_type_t {
            constexpr explicit in_place_type_t(std::int_fast32_t) noexcept {}
        };

        template<class T> constexpr in_place_type_t<T> in_place_type{0};

        template<std::size_t I>
        struct in_place_index_t {
            constexpr explicit in_place_index_t(std::int_fast32_t) noexcept {}
        };

        template<std::size_t I> constexpr in_place_index_t<I> in_place_index{0};

    } //namespace cxx14ports::cxx17
} //namespace cxx14ports

namespace std { //NOLINT NOSONAR namespace std
    using ::cxx14ports::cxx17::in_place_t;
    using ::cxx14ports::cxx17::in_place;
    using ::cxx14ports::cxx17::in_place_type_t;
    using ::cxx14ports::cxx17::in_place_type;
    using ::cxx14ports::cxx17::in_place_index_t;
    using ::cxx14ports::cxx17::in_place_index;
}

#endif // (__cplusplus < 201700L) && (__cplusplus >= 201401L)

#endif //CXX14PORTS_CXX17_UTILITY_HPP
