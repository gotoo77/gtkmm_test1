
#ifndef UTILS_OPTIONAL_HPP
#define UTILS_OPTIONAL_HPP

#include <utils/type_traits.hpp>
#include <utils/utility.hpp>
#include <functional>
#include <exception>
#include <cstdint>

#if __cplusplus > 201700
#warning utils::optional is deprecated in C++17; use std::optional instead.
#endif // __cplusplus > 201700

namespace utils {

    struct nullopt_t {
        constexpr explicit nullopt_t(std::int_fast32_t) noexcept {}
    };

    constexpr nullopt_t nullopt{0};

    class bad_optional_access : public std::exception {
    public:
        const char *what() const noexcept override {
            return "bad optional access";
        }
    };

    template<class T>
    union __optional_trivial_storage {
        char dummy[sizeof(T)];
        T value;

        constexpr explicit __optional_trivial_storage() noexcept: dummy{} {}

        constexpr explicit __optional_trivial_storage(const T &value) : value(value) {}

        constexpr explicit __optional_trivial_storage(T &&value) noexcept: value(std::forward<T>(value)) {}

        template<typename ...Args>
        constexpr explicit __optional_trivial_storage(std::in_place_t, Args &&...args)
                : value(std::forward<Args>(args)...) {}
    };

    template<class T>
    union __optional_default_storage {
        char dummy[sizeof(T)];
        T value;

        constexpr explicit __optional_default_storage() noexcept: dummy{} {}

        constexpr explicit __optional_default_storage(const T &value) : value(value) {}

        constexpr explicit __optional_default_storage(T &&value) noexcept: value(std::forward<T>(value)) {}

        template<typename ...Args>
        constexpr explicit __optional_default_storage(std::in_place_t, Args &&...args)
                : value(std::forward<Args>(args)...) {}

        ~ __optional_default_storage() {} // NOLINT no =default here
    };

    template<class T>
    struct __optional_trivially_destructible_base {
        static_assert(std::is_trivially_destructible<T>::value);

        bool initialized_;
        __optional_trivial_storage<T> storage_;

        constexpr __optional_trivially_destructible_base() noexcept: initialized_(false), storage_() {}

        constexpr explicit __optional_trivially_destructible_base(const T &value)
                : initialized_(true), storage_(value) {}

        constexpr explicit __optional_trivially_destructible_base(T &&value) noexcept
                : initialized_(true), storage_(std::forward<T>(value)) {}

        template<typename ...Args>
        constexpr explicit __optional_trivially_destructible_base(std::in_place_t, Args &&...args)
                : initialized_(true), storage_(std::in_place, std::forward<Args>(args)...) {}
    };

    template<class T,
            bool CopyEnabled = std::conjunction<std::is_copy_constructible<T>, std::is_copy_assignable<T>>::value,
            bool MoveEnabled = std::conjunction<std::is_move_constructible<T>, std::is_move_assignable<T>>::value
    >
    struct __optional_default_base {
        bool initialized_;
        __optional_default_storage<T> storage_;

        constexpr __optional_default_base() noexcept: initialized_(false) {}

        constexpr explicit __optional_default_base(const T &value)
                : initialized_(true), storage_(value) {}

        constexpr explicit __optional_default_base(T &&value) noexcept
                : initialized_(true), storage_(std::forward<T>(value)) {}

        template<typename ...Args>
        constexpr explicit __optional_default_base(std::in_place_t, Args &&...args)
                : initialized_(true), storage_(std::in_place, std::forward<Args>(args)...) {}

        constexpr __optional_default_base(const __optional_default_base<T, CopyEnabled, MoveEnabled> &rhs)
                : initialized_(rhs.initialized_) {
            if (rhs.initialized_) {
                new(&storage_.value) T(rhs.storage_.value);
            }
        }

        constexpr __optional_default_base(__optional_default_base<T, CopyEnabled, MoveEnabled> &&rhs) noexcept
                : initialized_(rhs.initialized_) {
            if (rhs.initialized_) {
                new(&storage_.value) T(std::forward<T>(rhs.storage_.value));
            }
        }

        ~ __optional_default_base() {
            if (initialized_) {
                storage_.value.T::~T();
            }
        }

        constexpr __optional_default_base<T, CopyEnabled, MoveEnabled> &
        operator=(const __optional_default_base<T, CopyEnabled, MoveEnabled> &rhs) {
            if (&rhs != this) {
                if (initialized_) {
                    storage_.value.T::~T();
                }
                initialized_ = rhs.initialized_;
                if (rhs.initialized_) {
                    new(&storage_.value) T(rhs.storage_.value);
                }
            }
            return *this;
        }

        constexpr __optional_default_base<T, CopyEnabled, MoveEnabled> &
        operator=(__optional_default_base<T, CopyEnabled, MoveEnabled> &&rhs) noexcept {
            if (&rhs != this) {
                if (initialized_) {
                    storage_.value.T::~T();
                }
                initialized_ = rhs.initialized_;
                if (rhs.initialized_) {
                    new(&storage_.value) T(std::forward<T>(rhs.storage_.value));
                }
            }
            return *this;
        }
    };

    template<class T>
    struct __optional_default_base<T, false, true> : __optional_default_base<T, true, true> {
        using __base_type = __optional_default_base<T, true, true>;
        using __this_type = __optional_default_base<T, false, true>;

        using __base_type::__optional_default_base;

        constexpr __optional_default_base() noexcept = default;

        ~ __optional_default_base() = default;

        __optional_default_base(const __this_type &) = delete;

        __this_type &operator=(const __this_type &) = delete;

        __optional_default_base(__this_type &&) noexcept = default;

        __this_type &operator=(__this_type &&) noexcept = default;
    };

    template<class T>
    struct __optional_default_base<T, true, false> : __optional_default_base<T, true, true> {
        using __base_type = __optional_default_base<T, true, true>;
        using __this_type = __optional_default_base<T, true, false>;

        using __base_type::__optional_default_base;

        constexpr __optional_default_base() noexcept = default;

        ~ __optional_default_base() = default;

        __optional_default_base(const __this_type &) = default;

        __this_type &operator=(const __this_type &) = default;

        __optional_default_base(__this_type &&) = delete;

        __this_type &operator=(__this_type &&) noexcept = delete;
    };

    template<class T>
    struct __optional_default_base<T, false, false> : __optional_default_base<T, true, true> {
        using __base_type = __optional_default_base<T, true, true>;
        using __this_type = __optional_default_base<T, false, false>;

        using __base_type::__optional_default_base;

        constexpr __optional_default_base() noexcept = default;

        ~ __optional_default_base() = default;

        __optional_default_base(const __this_type &) = delete;

        __this_type &operator=(const __this_type &) = delete;

        __optional_default_base(__this_type &&) = delete;

        __this_type &operator=(__this_type &&) noexcept = delete;
    };

    template<class T>
    using __optional_base = std::conditional_t<std::is_trivially_destructible<T>::value,
            __optional_trivially_destructible_base<T>,
            __optional_default_base<T>>;

    template<class T>
    class optional : private __optional_base<T> {
        static_assert(std::negation<std::is_same<T, nullopt_t>>::value);
        static_assert(std::negation<std::is_same<T, std::in_place_t>>::value);

        constexpr void clear() noexcept {
            __optional_base<T>::storage_.value.T::~T();
            __optional_base<T>::initialized_ = false;

        }

        constexpr bool initialized() const noexcept {
            return __optional_base<T>::initialized_;
        }

        template<class U>
        constexpr void initialize(const U &v) {
            new(&__optional_base<T>::storage_.value) T(v);
            __optional_base<T>::initialized_ = true;
        }

        template<class U>
        constexpr void initialize(U &&v)
        noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_constructible<T>::value) {
            new(&__optional_base<T>::storage_.value) T(std::forward<U>(v));
            __optional_base<T>::initialized_ = true;
        }

        template<typename ...Args>
        constexpr T &initialize(std::in_place_t, Args &&...args) {
            new(&__optional_base<T>::storage_.value) T(std::forward<Args>(args)...);
            __optional_base<T>::initialized_ = true;
            return __optional_base<T>::storage_.value;
        }

        template<class U>
        constexpr void set_value(const U &v) {
            __optional_base<T>::storage_.value = v;
        }

        template<class U>
        constexpr void set_value(U &&v) {
            __optional_base<T>::storage_.value = std::forward<U>(v);
        }

        constexpr const T &get_value() const &{
            return __optional_base<T>::storage_.value;
        }

        constexpr T &get_value() &{
            return __optional_base<T>::storage_.value;
        }

        constexpr T &&get_value() const &&{
            return std::move(__optional_base<T>::storage_.value);
        }

        constexpr T &&get_value() &&{
            return std::move(__optional_base<T>::storage_.value);
        }

        template<class U>
        constexpr void assign(const optional<U> &rhs) {
            if (initialized() && !rhs.initialized()) {
                clear();
            } else if (!initialized() && rhs.initialized()) {
                initialize(rhs);
            } else if (initialized() && rhs.initialized()) {
                set_value(rhs.value());
            }
        }

        constexpr void assign(optional &&rhs)
        noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_constructible<T>::value) {
            if (&rhs != this) {
                if (initialized() && !rhs.initialized()) {
                    clear();
                } else if (!initialized() && rhs.initialized()) {
                    initialize(std::move(rhs.value()));
                } else if (initialized() && rhs.initialized()) {
                    set_value(std::move(rhs.value()));
                }
            }
        }

    public:
        using value_type = T;

        constexpr optional() = default;

        constexpr optional(nullopt_t) {} //NOLINT NOSONAR explicit

        constexpr explicit optional(const T &value) : __optional_base<T>(value) {}

        constexpr explicit optional(T &&value) noexcept(std::is_nothrow_move_constructible<T>::value)
                : __optional_base<T>(std::forward<T>(value)) {}

        template<typename ...Args>
        constexpr explicit optional(std::in_place_t, Args &&...args)
                : __optional_base<T>(std::in_place, std::forward<Args>(args)...) {}

        constexpr optional(const optional &o)
                : __optional_base<T>(o) {}

        constexpr optional(optional &&o) noexcept
                : __optional_base<T>(std::move(o)) {}

        template<class U, typename = std::enable_if_t<std::negation<std::is_same<U, T>>::value>>
        constexpr explicit optional(const optional<U> &o)
                : __optional_base<T>() {
            if (o.initialized()) {
                initialize(o.value());
            }
        }

        template<class U, typename = std::enable_if_t<std::negation<std::is_same<U, T>>::value>>
        constexpr explicit optional(optional<U> &&o) noexcept(std::is_nothrow_move_assignable<T>::value)
                : __optional_base<T>() {
            if (o.has_value()) {
                initialize(std::move(o.value()));
                o.reset();
            }
        }

        constexpr optional &operator=(nullopt_t) noexcept {
            if (initialized()) {
                clear();
            }
            return *this;
        }

        constexpr optional &operator=(const optional &rhs)
        noexcept(std::is_nothrow_copy_assignable<T>::value && std::is_nothrow_copy_assignable<T>::value) {
            if (&rhs != this) {
                optional<T>(rhs).swap(*this);
            }
            return *this;
        }

        constexpr optional &operator=(optional &&rhs)
        noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_constructible<T>::value) {
            if (&rhs != this) {
                optional<T>(std::move(rhs)).swap(*this);
            }
            return *this;
        }

        constexpr const T *operator->() const { return &get_value(); }

        constexpr T *operator->() { return &get_value(); }

        constexpr const T &operator*() const &{ return get_value(); }

        constexpr T &operator*() &{ return get_value(); }

        constexpr const T &&operator*() const &&{ return std::move(get_value()); }

        constexpr T &&operator*() &&{ return std::move(get_value()); }

        constexpr bool has_value() const noexcept { return initialized(); }

        constexpr explicit operator bool() const noexcept { return initialized(); }

        constexpr const T &value() const &{ return bool(*this) ? **this : (throw bad_optional_access(), **this); }

        constexpr T &value() &{ return bool(*this) ? **this : (throw bad_optional_access(), **this); }

        constexpr const T &&value() const &&{
            return bool(*this) ? std::move(**this) : (throw bad_optional_access(), **this);
        }

        constexpr T &&value() &&{ return bool(*this) ? std::move(**this) : (throw bad_optional_access(), **this); }

        template<class U>
        constexpr T value_or(U &&default_value) const &{
            return bool(*this) ? **this : static_cast<T>(std::forward<U>(default_value));
        }

        template<class U>
        constexpr T value_or(U &&default_value) &&{
            return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(default_value));
        }

        constexpr void swap(optional &rhs) noexcept {
            if (&rhs != this && (initialized() || rhs.initialized())) {
                if (initialized() && !rhs.initialized()) {
                    rhs.initialize(std::move(get_value()));
                    clear();
                } else if (!initialized() && rhs.initialized()) {
                    initialize(std::move(rhs.get_value()));
                    rhs.clear();
                } else if (initialized() && rhs.initialized()) {
                    std::swap(get_value(), rhs.get_value());
                }
            }
        }

        void reset() noexcept {
            if (initialized()) {
                clear();
            }
        }

        template<typename ...Args>
        constexpr T &emplace(Args &&... args) {
            if (initialized()) {
                clear();
            }
            return initialize(std::in_place, std::forward<Args>(args)...);
        }
    };

    template<class T>
    constexpr optional<std::decay_t<T>> make_optional(nullopt_t) {
        return nullopt;
    }

    template<class T>
    constexpr optional<std::decay_t<T>> make_optional(T &&value) {
        return optional<std::decay_t<T>>(std::forward<T>(value));
    }

    template<class T, typename ...Args>
    constexpr optional<T> make_optional(Args &&...args) {
        return optional<T>(std::in_place, std::forward<Args>(args)...);
    }

} // namespace utils

namespace std { //NOLINT NOSONAR std

    template<class T>
    constexpr void swap(utils::optional<T> &lhs, utils::optional<T> &rhs) noexcept {
        lhs.swap(rhs);
    }

    template<>
    struct hash<utils::nullopt_t> {
        constexpr std::size_t operator()(utils::nullopt_t) const noexcept {
            return static_cast<size_t>(0);
        }
    };

    template<class T>
    struct hash<utils::optional<T>> : hash<std::remove_const_t<T>> {
        template<typename = void_t<decltype(hash<std::remove_const_t<T>>()(declval<remove_const_t<T>>()))>>
        constexpr size_t operator()(const utils::optional<T> &opt) const {
            return opt ? hash<remove_const_t<T>>()(*opt) : hash<utils::nullopt_t>()(utils::nullopt);
        }
    };

} //namespace std

namespace utils {

    template<class T, class U>
    constexpr bool operator==(const utils::optional<T> &lhs, const utils::optional<U> &rhs) {
        return (!bool(lhs) && !bool(rhs)) || (bool(lhs) && bool(rhs) && *lhs == *rhs);
    }

    template<class T, class U>
    constexpr bool operator!=(const utils::optional<T> &lhs, const utils::optional<U> &rhs) {
        return !(lhs == rhs);
    }

    template<class T, class U>
    constexpr bool operator<(const utils::optional<T> &lhs, const utils::optional<U> &rhs) {
        return (!bool(lhs) && bool(rhs)) || (bool(lhs) && bool(rhs) && lhs < rhs);
    }

    template<class T, class U>
    constexpr bool operator<=(const utils::optional<T> &lhs, const utils::optional<U> &rhs) {
        return (lhs == rhs) || (lhs < rhs);
    }

    template<class T, class U>
    constexpr bool operator>(const utils::optional<T> &lhs, const utils::optional<U> &rhs) {
        return !(lhs <= rhs);
    }

    template<class T, class U>
    constexpr bool operator>=(const utils::optional<T> &lhs, const utils::optional<U> &rhs) {
        return !(lhs < rhs);
    }


    template<class T>
    constexpr bool operator==(const utils::optional<T> &opt, utils::nullopt_t) noexcept {
        return !(opt);
    }

    template<class T>
    constexpr bool operator==(utils::nullopt_t, const utils::optional<T> &opt) noexcept {
        return !(opt);
    }

    template<class T>
    constexpr bool operator!=(const utils::optional<T> &opt, utils::nullopt_t) noexcept {
        return bool(opt);
    }

    template<class T>
    constexpr bool operator!=(utils::nullopt_t, const utils::optional<T> &opt) noexcept {
        return bool(opt);
    }

    template<class T>
    constexpr bool operator<(const utils::optional<T> &opt, utils::nullopt_t) noexcept {
        return false;
    }

    template<class T>
    constexpr bool operator<(utils::nullopt_t, const utils::optional<T> &opt) noexcept {
        return opt != utils::nullopt;
    }

    template<class T>
    constexpr bool operator<=(const utils::optional<T> &opt, utils::nullopt_t) noexcept {
        return opt == utils::nullopt;
    }

    template<class T>
    constexpr bool operator<=(utils::nullopt_t, const utils::optional<T> &opt) noexcept {
        return true;
    }

    template<class T>
    constexpr bool operator>(const utils::optional<T> &opt, utils::nullopt_t) noexcept {
        return opt != utils::nullopt;
    }

    template<class T>
    constexpr bool operator>(utils::nullopt_t, const utils::optional<T> &opt) noexcept {
        return false;
    }

    template<class T>
    constexpr bool operator>=(const utils::optional<T> &opt, utils::nullopt_t) noexcept {
        return true;
    }

    template<class T>
    constexpr bool operator>=(utils::nullopt_t, const utils::optional<T> &opt) noexcept {
        return opt == utils::nullopt;
    }

    template<class T, class U>
    constexpr bool operator==(const utils::optional<T> &opt, const U &value) {
        return bool(opt) && *opt == value;
    }

    template<class T, class U>
    constexpr bool operator==(const T &value, const utils::optional<U> &opt) {
        return bool(opt) && *opt == value;
    }

    template<class T, class U>
    constexpr bool operator!=(const utils::optional<T> &opt, const U &value) {
        return !(opt) || *opt != value;
    }

    template<class T, class U>
    constexpr bool operator!=(const T &value, const utils::optional<U> &opt) {
        return !(opt) || *opt != value;
    }

    template<class T, class U>
    constexpr bool operator<(const utils::optional<T> &opt, const U &value) {
        return !(opt) || *opt < value;
    }

    template<class T, class U>
    constexpr bool operator<(const T &value, const utils::optional<U> &opt) {
        return bool(opt) && value < *opt;
    }

    template<class T, class U>
    constexpr bool operator<=(const utils::optional<T> &opt, const U &value) {
        return !(opt) || *opt <= value;
    }

    template<class T, class U>
    constexpr bool operator<=(const T &value, const utils::optional<U> &opt) {
        return bool(opt) && value <= *opt;
    }

    template<class T, class U>
    constexpr bool operator>(const utils::optional<T> &opt, const U &value) {
        return bool(opt) && *opt > value;
    }

    template<class T, class U>
    constexpr bool operator>(const T &value, const utils::optional<U> &opt) {
        return bool(opt) && value > *opt;
    }

    template<class T, class U>
    constexpr bool operator>=(const utils::optional<T> &opt, const U &value) {
        return bool(opt) && *opt >= value;
    }

    template<class T, class U>
    constexpr bool operator>=(const T &value, const utils::optional<U> &opt) {
        return !(opt) || value >= *opt;
    }

} // namespace utils

#endif //UTILS_OPTIONAL_HPP
