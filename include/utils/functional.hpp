
#ifndef UTILS_FUNCTIONAL_HPP
#define UTILS_FUNCTIONAL_HPP

#include <functional>

namespace utils {

    template<class T>
    struct true_predicate {
        constexpr bool operator()(const T &) const noexcept {
            return true;
        }
    };

    template<>
    struct true_predicate<void> {
        template<class T>
        constexpr bool operator()(T &&) const noexcept {
            return true;
        }
    };


    template<class T>
    struct false_predicate {
        constexpr bool operator()(const T &) const noexcept {
            return false;
        }
    };

    template<>
    struct false_predicate<void> {
        template<class T>
        constexpr bool operator()(T &&) const noexcept {
            return false;
        }
    };

    template<class Comparator, class T>
    class comparator_predicate {
        T _value;

    public:
        inline explicit comparator_predicate(T value) : _value(value) {}

        inline
        bool operator()(const typename std::decay_t<T> &other) const {
            return Comparator()(_value, other);
        }
    };

} // namespace utils

#endif //UTILS_FUNCTIONAL_HPP
