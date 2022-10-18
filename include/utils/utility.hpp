
#ifndef UTILS_UTILITY_HPP
#define UTILS_UTILITY_HPP

#include <utility>

#if (__cplusplus < 201400L) && (__cplusplus >= 201103L)

#include "cxx11ports/cxx14/utility.hpp"

#endif

#if (__cplusplus < 201700L) && (__cplusplus >= 201401L)

#include <cxx14ports/cxx17/utility.hpp>

#endif // (__cplusplus < 201700L) && (__cplusplus >= 201401L)

#endif //UTILS_UTILITY_HPP
