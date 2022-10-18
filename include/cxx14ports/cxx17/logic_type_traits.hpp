
#ifndef CXX14PORTS_CXX17_LOGIC_TYPE_TRAITS_HPP
#define CXX14PORTS_CXX17_LOGIC_TYPE_TRAITS_HPP

#include <type_traits>

#if (__cplusplus < 201700L) && (__cplusplus >= 201401L)

namespace cxx14ports {
    namespace cxx17 {

        template<class...>
        struct conjunction : std::true_type {
        };
        template<class B1, class... Bn>
        struct conjunction<B1, Bn...>
                : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {
        };

        template<class...>
        struct disjunction : std::false_type {
        };
        template<class B1, class... Bn>
        struct disjunction<B1, Bn...>
                : std::conditional<bool(B1::value), B1, disjunction<Bn...>>::type {
        };

        template<class B>
        struct negation : std::integral_constant<bool, !bool(B::value)> {
        };

    } // namespace cxx14ports::cxx17
} // namespace cxx14ports

#endif // (__cplusplus < 201700L) && (__cplusplus >= 201401L

#endif //CXX14PORTS_CXX17_LOGIC_TYPE_TRAITS_HPP
