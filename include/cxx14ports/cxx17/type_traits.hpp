
#ifndef CXX11PORTS_CXX17_TYPE_TRAITS_H
#define CXX11PORTS_CXX17_TYPE_TRAITS_H

#include <type_traits>

// C++ 17 port. C++ 17 compiler should already have that defined
// written for C++ 14 compliant compiler. Pre-C++11 shouldn't have that defined
#if (__cplusplus < 201700L) && (__cplusplus >= 201401L)

#include <cxx14ports/cxx17/logic_type_traits.hpp>

namespace std { // NOLINT - modification of 'std' namespace can result in undefined behavior
    template<bool B>
    using bool_constant = ::std::integral_constant<bool, B>;

#if (__GNUG__ && not __STRICT_ANSI__)
// gnu++14 seems to have void_t already defined
#else
    // strict c++14 compiler
    template<class ...Ts>
    using void_t = void;
#endif //(__GNUG__ && not __STRICT_ANSI__)

    using ::cxx14ports::cxx17::conjunction;
    using ::cxx14ports::cxx17::disjunction;
    using ::cxx14ports::cxx17::negation;

}

#endif //(__cplusplus < 201700L) && (__cplusplus >= 201103L)

#endif //CXX11PORTS_CXX17_TYPE_TRAITS_H
