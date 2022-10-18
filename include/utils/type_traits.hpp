
#ifndef UTILS_TYPE_TRAITS_H
#define UTILS_TYPE_TRAITS_H

#include <type_traits>

#if (__cplusplus < 201400L) && (__cplusplus >= 201103L)

#include "cxx11ports/cxx14/type_traits.hpp"
#include "cxx11ports/cxx17/type_traits.hpp"

#endif

#if (__cplusplus < 201700L) && (__cplusplus >= 201401L)

#include <cxx14ports/cxx17/type_traits.hpp>

#endif

#endif //UTILS_TYPE_TRAITS_H
