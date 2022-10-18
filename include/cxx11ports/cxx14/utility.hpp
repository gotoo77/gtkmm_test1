
#ifndef CXX11PORTS_CXX14__UTILITY_HPP
#define CXX11PORTS_CXX14__UTILITY_HPP

// C++ 14 port C++ 14 compiler should already have that defined
// written for C++ 11 compliant compiler. Pre-C++11 shouldn't have that defined
#if (__cplusplus < 201400L) && (__cplusplus >= 201103L)

#include <utility>
#include "cxx11ports/cxx14/integer_sequence.hpp"

namespace std { // NOLINT - modification of 'std' namespace can result in undefined behavior

// from cxx14_integer_sequence.h
using ::cxx11ports::cxx14::integer_sequence;
using ::cxx11ports::cxx14::index_sequence ;
using ::cxx11ports::cxx14::make_integer_sequence;
using ::cxx11ports::cxx14::make_index_sequence;
using ::cxx11ports::cxx14::index_sequence_for;

} // namespace std

#endif //(__cplusplus < 201400L) && (_cplusplus >= 201103L)

#endif //CXX11PORTS_CXX14__UTILITY_HPP
