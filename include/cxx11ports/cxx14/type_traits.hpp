
#ifndef CXX11PORTS_CXX14_TYPE_TRAITS_H
#define CXX11PORTS_CXX14_TYPE_TRAITS_H

#include <type_traits>
#include "cxx11ports/cxx14/integer_sequence.hpp"

// C++ 14 port. C++ 14 compiler should already have that defined
// written for C++ 11 compliant compiler. Pre-C++11 shouldn't have that defined
#if (__cplusplus < 201400L) && (__cplusplus >= 201103L)

namespace std { // NOLINT - modification of 'std' namespace can result in undefined behavior

// extending the existing std
template<class T>
using decay_t = typename ::std::decay<T>::type;
template<bool B, class T = void>
using enable_if_t = typename ::std::enable_if<B, T>::type;
template<bool B, class T, class F>
using conditional_t = typename ::std::conditional<B, T, F>::type;
template<class... T>
using common_type_t = typename ::std::common_type<T...>::type;
template<class T>
using underlying_type_t = typename ::std::underlying_type<T>::type;
template<class T>
using result_of_t = typename ::std::result_of<T>::type;
template<class T>
using remove_cv_t = typename ::std::remove_cv<T>::type;
template<class T>
using remove_const_t = typename ::std::remove_const<T>::type;
template<class T>
using remove_volatile_t = typename ::std::remove_volatile<T>::type;
template<class T>
using remove_reference_t = typename ::std::remove_reference<T>::type;
template<class T>
using add_cv_t = typename ::std::add_cv<T>::type;
template<class T>
using add_const_t = typename ::std::add_const<T>::type;
template<class T>
using add_volatile_t = typename ::std::add_volatile<T>::type;
template<class T>
using add_lvalue_reference_t = typename ::std::add_lvalue_reference<T>::type;
template<class T>
using add_rvalue_reference_t = typename ::std::add_rvalue_reference<T>::type;

} // namespace std

#endif //(__cplusplus < 201400L) && (__cplusplus >= 201103L)
#endif //CXX11PORTS_CXX14_TYPE_TRAITS_H
