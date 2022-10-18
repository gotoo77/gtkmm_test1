
#ifndef UTILS_ARRAY_MAP_HPP
#define UTILS_ARRAY_MAP_HPP

#include <utils/type_traits.hpp>
#include <utils/ignore_unused.hpp>
#include <utils/optional.hpp>
#include <array>
#include <tuple>

namespace utils {
    template<class K, class V, class Index, K ... Keys>
    class array_map;
}

namespace std { //NOLINT NOSONAR
    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr pair<const K, ::utils::optional<V>> &get(::utils::array_map<K, V, Index, Keys...> &am) noexcept;

    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr pair<const K, ::utils::optional<V>> &&get(::utils::array_map<K, V, Index, Keys...> &&am) noexcept;

    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr const pair<const K, ::utils::optional<V>> &
    get(const ::utils::array_map<K, V, Index, Keys...> &am) noexcept;

    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr const pair<const K, ::utils::optional<V>> &&
    get(const ::utils::array_map<K, V, Index, Keys...> &&am) noexcept;
}

namespace utils {
    namespace detail {

        template<class K, class V, K Key>
        constexpr
        utils::optional<V> array_map_fetch_value() {
            ignore_unused(Key);
            return nullopt;
        }

        template<class K, class V, K Key, typename ...Pairs>
        constexpr
        utils::optional<V> array_map_fetch_value(std::tuple<K, V> &&p, Pairs &&...others) {
            return (std::get<0>(p) == Key) ? make_optional(std::get<1>(p)) : array_map_fetch_value<K, V, Key>(
                    std::forward<Pairs>(others)...);
        }

        template<class K, class V, K Key, typename ...Pairs>
        constexpr
        utils::optional<V> array_map_fetch_value(std::pair<K, V> &&p, Pairs &&...others) {
            return (std::get<0>(p) == Key) ? make_optional(std::get<1>(p)) : array_map_fetch_value<K, V, Key>(
                    std::forward<Pairs>(others)...);
        }

    } // namespace utils::detail
} // namespace utils


namespace utils {

    template<class K, class V, class Index, K ... Keys>
    class array_map {
    public:
        constexpr static std::size_t N = sizeof...(Keys);

        static_assert(std::is_same<std::make_index_sequence<N>, std::index_sequence<Index()(Keys)...>>::value,
                      "Keys are not compatible with the provided Index");

        using key_type = K;
        using mapped_type = V;
        using value_type = std::pair<const K, optional<V>>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using indexer = Index;

        template<class It>
        class _iterator {
            static_assert(
                    std::is_base_of<std::bidirectional_iterator_tag, typename std::iterator_traits<It>::iterator_category>::value);
            // pair<K, optional<V>>

            It it_;
            It start_;
            It end_;

        public:
            using value_type = typename std::iterator_traits<It>::value_type;
            using pointer = typename std::iterator_traits<It>::pointer;
            using reference = typename std::iterator_traits<It>::reference;
            using difference_type = typename std::iterator_traits<It>::difference_type;
            using iterator_category = std::bidirectional_iterator_tag;

            constexpr
            _iterator(It it, It start, It end)
                    : it_(it), start_(start), end_(end) {
            }

            inline
            _iterator &operator++() {
                while (it_ != end_ && !((++it_)->second));
                return *this;
            }

            inline
            _iterator &operator--() {
                while (it_ != start_ && !((++it_)->second));
                return *this;
            }

            constexpr
            bool operator==(_iterator b) const {
                return it_ == b.it_;
            }

            constexpr
            bool operator!=(_iterator b) const {
                return it_ != b.it_;
            }

            constexpr
            reference operator*() const {
                return *it_;
            }

            constexpr
            pointer operator->() const {
                return it_;
            }

            constexpr difference_type operator-(_iterator rhs) const {
                return it_ - rhs.it_;
            }
        };

        using reference = typename std::array<value_type, N>::reference;
        using const_reference = typename std::array<value_type, N>::const_reference;
        using pointer = typename std::array<value_type, N>::pointer;
        using const_pointer = typename std::array<value_type, N>::const_pointer;

        using _base_iterator = typename std::array<value_type, N>::iterator;
        using _base_const_iterator = typename std::array<value_type, N>::const_iterator;
        using _base_reverse_iterator = typename std::array<value_type, N>::reverse_iterator;
        using _base_const_reverse_iterator = typename std::array<value_type, N>::const_reverse_iterator;

        using iterator = _iterator<_base_iterator>;
        using const_iterator = _iterator<_base_const_iterator>;
        using reverse_iterator = _iterator<_base_reverse_iterator>;
        using reverse_const_iterator = _iterator<_base_const_reverse_iterator>;

        template<typename ... Args>
        constexpr
        array_map(Args &&...args) // NOLINT NOSONAR allow implicit construction
                : values_({
                                  std::make_pair(
                                          Keys,
                                          detail::array_map_fetch_value<K, V, Keys>(std::forward<Args>(args)...)
                                  )...
                          }) {
        }

        iterator begin() noexcept { return _iterator<_base_iterator>(values_.begin(), values_.begin(), values_.end()); }

        iterator end() noexcept { return _iterator<_base_iterator>(values_.end(), values_.begin(), values_.end()); }

        constexpr const_iterator cbegin() const noexcept {
            return _iterator<_base_const_iterator>(values_.cbegin(), values_.cbegin(), values_.cend());
        }

        constexpr const_iterator cend() const noexcept {
            return _iterator<_base_const_iterator>(values_.cend(), values_.cend(), values_.cend());
        }

        constexpr const_iterator begin() const noexcept { return cbegin(); }

        constexpr const_iterator end() const noexcept { return cend(); }

        reverse_iterator rbegin() noexcept {
            return _iterator<_base_reverse_iterator>(values_.rbegin(), values_.rbegin(), values_.rend());
        }

        reverse_iterator rend() noexcept {
            return _iterator<_base_reverse_iterator>(values_.rbegin(), values_.rend(), values_.rend());
        }

        constexpr reverse_const_iterator crbegin() const noexcept {
            return _iterator<_base_const_reverse_iterator>(values_.crbegin(), values_.crbegin(), values_.crend());
        }

        constexpr reverse_const_iterator crend() const noexcept {
            return _iterator<_base_const_reverse_iterator>(values_.crbegin(), values_.crend(), values_.crend());
        }

        constexpr reverse_const_iterator rbegin() const noexcept { return crbegin(); }

        constexpr reverse_const_iterator rend() const noexcept { return crend(); }

        constexpr bool empty() const noexcept { return values_.empty(); }

        constexpr size_type size() const noexcept { return values_.size(); }

        constexpr size_type max_size() const noexcept { return values_.max_size(); }

        reference at(K key) { return values_.at(indexer()(key)); }

        constexpr const_reference at(K key) const { return values_.at(indexer()(key)); }

        reference operator[](K key) { return values_[indexer()(key)]; }

        constexpr const_reference operator[](K key) const { return values_[indexer()(key)]; }

        template<typename = std::enable_if_t<std::negation<std::is_same<K, std::size_t>>::value>>
        reference at(std::size_t index) { return values_.at(index); }

        template<typename = std::enable_if_t<std::negation<std::is_same<K, std::size_t>>::value>>
        constexpr const_reference at(std::size_t index) const { return values_.at(index); }

        template<typename = std::enable_if_t<std::negation<std::is_same<K, std::size_t>>::value>>
        reference operator[](std::size_t index) { return values_[index]; }

        template<typename = std::enable_if_t<std::negation<std::is_same<K, std::size_t>>::value>>
        constexpr const_reference operator[](std::size_t index) const { return values_[index]; }

        reference front() { return values_.front(); }

        constexpr const_reference front() const { return values_.front(); }

        reference back() { return values_.back(); }

        constexpr const_reference back() const { return values_.back(); }

        void swap(array_map &other) noexcept { values_.swap(other.values_); }

    private:
        std::array<value_type, N> values_;

    public:
        template<::std::size_t I, class K1, class V1, class Index1, K1 ... Keys1>
        friend constexpr ::std::pair<const K1, ::utils::optional<V1>> &
        ::std::get(::utils::array_map<K1, V1, Index1, Keys1...> &ma) noexcept;

        template<::std::size_t I, class K1, class V1, class Index1, K1 ... Keys1>
        friend constexpr ::std::pair<const K1, ::utils::optional<V1>> &&
        ::std::get(::utils::array_map<K1, V1, Index1, Keys1...> &&ma) noexcept;

        template<::std::size_t I, class K1, class V1, class Index1, K1 ... Keys1>
        friend constexpr const ::std::pair<const K1, ::utils::optional<V1>> &
        ::std::get(const ::utils::array_map<K1, V1, Index1, Keys1...> &ma) noexcept;

        template<::std::size_t I, class K1, class V1, class Index1, K1 ... Keys1>
        friend constexpr const ::std::pair<const K1, ::utils::optional<V1>> &&
        ::std::get(const ::utils::array_map<K1, V1, Index1, Keys1...> &&ma) noexcept;
    };

    template<class K, K Key, class V, class Index, K ... Keys>
    constexpr optional<V> &get_value(::utils::array_map<K, V, Index, Keys...> &am) noexcept {
        static_assert(std::disjunction<std::bool_constant<Key == Keys>...>::value, "Key out of bounds");

        return ::std::get<Index()(Key)>(am).second;
    }

    template<class K, K Key, class V, class Index, K ... Keys>
    constexpr optional<V> &&get_value(::utils::array_map<K, V, Index, Keys...> &&am) noexcept {
        static_assert(std::disjunction<std::bool_constant<Key == Keys>...>::value, "Key out of bounds");

        return ::std::get<Index()(Key)>(am).second;
    }

    template<class K, K Key, class V, class Index, K ... Keys>
    constexpr const optional<V> &get_value(const ::utils::array_map<K, V, Index, Keys...> &am) noexcept {
        static_assert(std::disjunction<std::bool_constant<Key == Keys>...>::value, "Key out of bounds");

        return ::std::get<Index()(Key)>(am).second;
    }

    template<class K, K Key, class V, class Index, K ... Keys>
    constexpr const optional<V> &&get_value(const ::utils::array_map<K, V, Index, Keys...> &&am) noexcept {
        static_assert(std::disjunction<std::bool_constant<Key == Keys>...>::value, "Key out of bounds");

        return ::std::get<Index()(Key)>(am).second;
    }

    template<class K>
    struct integral_index {
        static_assert(std::is_integral<K>::value);

        constexpr std::size_t operator()(K key) const noexcept {
            return static_cast<std::size_t>(key);
        }
    };

    template<class E>
    struct enum_index {
        static_assert(std::is_enum<E>::value);

        constexpr std::size_t operator()(E e) const noexcept {
            using underlying_type = typename std::underlying_type<E>::type;
            return static_cast<std::size_t>(static_cast<underlying_type>(e));
        }
    };

    template<class I, class V, I ... Keys>
    using integral_array_map = array_map<I, V, integral_index<I>, Keys...>;

    template<class E, class V, E ... Keys>
    using enum_array_map = array_map<E, V, enum_index<E>, Keys...>;

} // namespace utils

namespace std { //NOLINT NOSONAR std


    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr pair<const K, ::utils::optional<V>> &get(::utils::array_map<K, V, Index, Keys...> &am) noexcept {
        static_assert(I < ::utils::array_map<K, V, Index, Keys...>::N, "index I out of bounds");
        return get<I>(am.values_);
    }

    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr pair<const K, ::utils::optional<V>> &&get(::utils::array_map<K, V, Index, Keys...> &&am) noexcept {
        static_assert(I < ::utils::array_map<K, V, Index, Keys...>::N, "index I out of bounds");
        return get<I>(am.values_);
    }

    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr const pair<const K, ::utils::optional<V>> &
    get(const ::utils::array_map<K, V, Index, Keys...> &am) noexcept {
        static_assert(I < ::utils::array_map<K, V, Index, Keys...>::N, "index I out of bounds");
        return get<I>(am.values_);
    }

    template<size_t I, class K, class V, class Index, K ... Keys>
    constexpr const pair<const K, ::utils::optional<V>> &&
    get(const ::utils::array_map<K, V, Index, Keys...> &&am) noexcept {
        static_assert(I < ::utils::array_map<K, V, Index, Keys...>::N, "index I out of bounds");
        return get<I>(am.values_);
    }


    template<class K, class V, class Index, K ... Keys>
    void swap(::utils::array_map<K, V, Index, Keys...> &lhs,
              ::utils::array_map<K, V, Index, Keys...> &rhs) {
        lhs.swap(rhs);
    }

} // namespace std

#endif //UTILS_ARRAY_MAP_HPP
