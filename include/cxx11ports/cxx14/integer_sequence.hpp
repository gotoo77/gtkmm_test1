
#ifndef CXX11PORTS_CXX14_INTEGER_SEQUENCE_H
#define CXX11PORTS_CXX14_INTEGER_SEQUENCE_H

#include <type_traits>

// C++ 14 port C++ 14 compiler should already have that defined
// written for C++ 11 compliant compiler. Pre-C++11 shouldn't have that defined
#if (__cplusplus < 201400L) && (__cplusplus >= 201103L)

/*
 * Implementation of std::integer_sequence from C++14
 *
 * std::integer_sequence<typename T, T... Ints>
 * compile time sequence of integer-like values
 *
 * std::make_integer_sequence<typename T, T N>
 * Compile time integer sequence generation, given a type 'T' and and integer-like value 'N'. 
 * Achieves O(log(N)) compile time complexity.
 *
 * std::make_index_sequence<size_t N>
 * Alias for std::make_integer_sequence<size_t, N>
 *
 * std::make_index_sequence_for<class... T>
 * Facility to make indexes for compile-time parameter pack.
 * Alias for std::make_index_sequence<sizeof...(T)>;
 */

namespace cxx11ports {
namespace cxx14 {

template<class T, T...>
struct seq
{
    using value_type = seq;
};

template<class T, class S1, class S2>
struct concat;
template<class T, T... S1, T... S2>
struct concat<T, seq<T, S1...>, seq<T, S2...> >
        : seq<T, S1..., (sizeof...(S1) + S2)...>
{
};

template<
        class T,
        typename std::enable_if<
                std::is_integral<T>::value and
                std::is_unsigned<T>::value,
                T>::type N
>
struct gen_seq;

template<
        class T,
        typename std::enable_if<
                std::is_integral<T>::value and
                std::is_unsigned<T>::value,
                T>::type N
>
struct gen_seq
        : concat<T, typename gen_seq<T, N / 2>::value_type, typename gen_seq<T, N - N / 2>::value_type>::value_type
{
};

template<class T>
struct gen_seq<T, 0>
        : seq<T>
{
};
template<class T>
struct gen_seq<T, 1>
        : seq<T, 0>
{
};

template<class T, T ...S>
using integer_sequence = seq<T, S...>;
template<std::size_t ...S>
using index_sequence = integer_sequence <std::size_t, S...>;

template<class T, T N>
using make_integer_sequence = gen_seq<T, N>;

template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template<class ...T>
using index_sequence_for = make_index_sequence<sizeof...(T)>;

} // namespace cxx11ports::cxx14
} // namespace cxx11ports

#endif //(__cplusplus < 201400L) && (_cplusplus >= 201103L)

#endif //CXX11PORTS_CXX14_INTEGER_SEQUENCE_H


