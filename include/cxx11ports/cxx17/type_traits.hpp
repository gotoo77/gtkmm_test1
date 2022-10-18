//
// Created by henris1 on 24/04/18.
//

#ifndef CXX11PORTS_CXX17_TYPE_TRAITS_H
#define CXX11PORTS_CXX17_TYPE_TRAITS_H

#include <type_traits>

// C++ 17 port. C++ 17 compiler should already have that defined
// written for C++ 11 compliant compiler. Pre-C++11 shouldn't have that defined
#if (__cplusplus < 201700L) && (__cplusplus >= 201103L)

namespace cxx11ports {
    namespace cxx17 {

#if (__GNUG__ && not __STRICT_ANSI__)
// gnu++11 seems to have void_t already defined
#else
        // strict c++11 compiler
        template<class ...>
        struct make_void
        {
            typedef void type;
        };
#endif //(__GNUG__ && not __STRICT_ANSI__)

    } // namespace cxx11ports::cxx17
} // namespace cxx11ports


namespace std { // NOLINT - modification of 'std' namespace can result in undefined behavior
    template<bool B>
    using bool_constant = ::std::integral_constant<bool, B>;

#if (__GNUG__ && not __STRICT_ANSI__)
// gnu++11 seems to have void_t already defined
#else
    // strict c++11 compiler
    template<class ...Ts>
    using void_t = typename ::cxx11ports::cxx17::make_void<Ts...>::type;
#endif //(__GNUG__ && not __STRICT_ANSI__)

}

#endif //(__cplusplus < 201700L) && (__cplusplus >= 201103L)

#endif //CXX11PORTS_CXX17_TYPE_TRAITS_H
