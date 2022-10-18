
#ifndef UTILS_STRING_VIEW_HPP
#define UTILS_STRING_VIEW_HPP

#include <bits/char_traits.h>
#include <algorithm>
#include <ostream>
#include <string>

namespace utils {

    template<typename CharT, typename Traits = std::char_traits<CharT> >
    class basic_string_view {
        static constexpr CharT zero = 0;

    public:
        using traits_type = Traits;
        using value_type = CharT;
        using pointer = CharT *;
        using const_pointer = const CharT *;
        using reference = CharT &;
        using const_reference = const CharT &;
        using const_iterator = const_pointer;
        using iterator = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        static constexpr size_type npos = static_cast<size_type>(-1);

    private:
        const_pointer str_;
        size_type size_;

    public:
        constexpr basic_string_view() noexcept
                : str_(nullptr), size_(0) {
        }

        constexpr basic_string_view(const basic_string_view &other) noexcept = default;

        constexpr basic_string_view(const CharT *s, size_type count)
                : str_(s), size_(count) {
        }

        constexpr basic_string_view(const CharT *begin, const CharT *end)
                : str_(begin), size_((begin != nullptr && end != nullptr) ? (end - begin) : 0) {
        }

        constexpr basic_string_view &operator=(const basic_string_view &view) noexcept = default;

        constexpr const_iterator begin() const noexcept {
            return cbegin();
        }

        constexpr const_iterator cbegin() const noexcept {
            return str_;
        }

        constexpr const_iterator end() const noexcept {
            return cend();
        }

        constexpr const_iterator cend() const noexcept {
            return (str_ != nullptr) ? str_ + size_ : nullptr;
        }

        constexpr const_reference operator[](size_type pos) const {
            return valid_access(pos) ? *(str_ + pos) : zero;
        }

        constexpr const_pointer data() const noexcept {
            return str_;
        }

        constexpr const_pointer c_str() const noexcept {
            return str_;
        }

        constexpr bool empty() const noexcept {
            return str_ == nullptr || size_ == 0;
        }

        constexpr size_type size() const noexcept {
            return str_ == nullptr ? 0 : size_;
        }

        constexpr size_type length() const noexcept {
            return size();
        }

        constexpr
        void swap(basic_string_view &sv) noexcept {
            if (&sv != this) {
                std::swap(str_, sv.str_);
                std::swap(size_, sv.size_);
            }
        }

        constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const {
            return valid_access(pos)
                   ? basic_string_view(str_ + pos, (count != npos && pos + count < size_
                                                    ? pos + count
                                                    : size_ - pos))
                   : basic_string_view();
        }

        size_type find(basic_string_view v, size_type pos = 0) const;

        size_type find(CharT ch, size_type pos = 0) const;

        size_type find(CharT *s, size_type pos, size_type count) const;

        size_type rfind(basic_string_view v, size_type pos = npos) const;

        size_type rfind(CharT ch, size_type pos = npos) const;

        size_type rfind(CharT *s, size_type pos, size_type count) const;

        size_type copy(CharT *dest, size_type count, size_type pos = 0) const;

        int compare(basic_string_view sv) const noexcept;

    private:
        constexpr bool valid_access(size_type pos) const noexcept {
            return str_ != nullptr && pos < size_;
        }
    };

    template<typename CharT, typename Traits>
    constexpr CharT basic_string_view<CharT, Traits>::zero;

} // namespace utils

template<class CharT, class Traits>
inline
bool operator==(utils::basic_string_view<CharT, Traits> lhs,
                utils::basic_string_view<CharT, Traits> rhs) noexcept {
    if (lhs.empty()) {
        return rhs.empty();
    }
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (size_t i = 0; i < lhs.length(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

template<class CharT, class Traits>
inline
bool operator<(utils::basic_string_view<CharT, Traits> lhs,
               utils::basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) < 0;
}

template<class CharT, class Traits>
inline
bool operator<=(utils::basic_string_view<CharT, Traits> lhs,
                utils::basic_string_view<CharT, Traits> rhs) noexcept {
    return lhs.compare(rhs) <= 0;
}

template<class CharT, class Traits, class Alloc>
inline
bool operator==(utils::basic_string_view<CharT, Traits> lhs,
                const std::basic_string<CharT, Traits, Alloc> &rhs) noexcept {
    return lhs == utils::basic_string_view<CharT, Traits>{rhs.c_str(), rhs.size()};
}

template<class CharT, class Traits, class Alloc>
inline
bool operator==(const std::basic_string<CharT, Traits, Alloc> &lhs,
                utils::basic_string_view<CharT, Traits> rhs) noexcept {
    return utils::basic_string_view<CharT, Traits>{lhs.c_str(), lhs.size()} == rhs;
}

template<class CharT, class Traits, class Alloc>
inline
bool operator<(utils::basic_string_view<CharT, Traits> lhs,
               const std::basic_string<CharT, Traits, Alloc> &rhs) noexcept {
    return lhs < utils::basic_string_view<CharT, Traits>{rhs.c_str(), rhs.size()};
}

template<class CharT, class Traits, class Alloc>
inline
bool operator<(const std::basic_string<CharT, Traits, Alloc> &lhs,
               utils::basic_string_view<CharT, Traits> rhs) noexcept {
    return utils::basic_string_view<CharT, Traits>{lhs.c_str(), lhs.size()} < rhs;
}

template<class CharT, class Traits, class Alloc>
inline
bool operator<=(utils::basic_string_view<CharT, Traits> lhs,
                const std::basic_string<CharT, Traits, Alloc> &rhs) noexcept {
    return lhs <= utils::basic_string_view<CharT, Traits>{rhs.c_str(), rhs.size()};
}

template<class CharT, class Traits, class Alloc>
inline
bool operator<=(const std::basic_string<CharT, Traits, Alloc> &lhs,
                utils::basic_string_view<CharT, Traits> rhs) noexcept {
    return utils::basic_string_view<CharT, Traits>{lhs.c_str(), lhs.size()} < rhs;
}

namespace utils {

    template<typename CharT, typename Traits>
    inline
    auto basic_string_view<CharT, Traits>::find(basic_string_view<CharT, Traits> v, size_type pos) const -> size_type {
        if (!valid_access(pos)) {
            return npos;
        }
        const size_type len = v.length();
        if (v.empty() || len > size_) {
            return npos;
        }

        for (size_type i = pos; i < size_ - len; ++i) {
            if (substr(i, len) == v) {
                return pos;
            }
        }

        return npos;
    }

    template<typename CharT, typename Traits>
    inline
    auto basic_string_view<CharT, Traits>::find(CharT ch, size_type pos) const -> size_type {
        return find(basic_string_view(std::addressof(ch), 1), pos);
    }

    template<typename CharT, typename Traits>
    inline
    auto basic_string_view<CharT, Traits>::find(CharT *s, size_type pos, size_type count) const -> size_type {
        return find(basic_string_view(s, count), pos);
    }

    template<typename CharT, typename Traits>
    inline
    auto basic_string_view<CharT, Traits>::rfind(basic_string_view<CharT, Traits> v, size_type pos) const -> size_type {
        if (!valid_access(pos)) {
            return npos;
        }
        constexpr size_type len = v.length();
        if (v.empty() || len > size_) {
            return npos;
        }

        for (size_type i = size_ - len - 1; i >= 0; --i) {
            if (substr(i, len) == v) {
                return pos;
            }
        }

        return npos;
    }

    template<typename CharT, typename Traits>
    inline
    auto basic_string_view<CharT, Traits>::rfind(CharT ch, size_type pos) const -> size_type {
        return find(basic_string_view(std::addressof(ch), 1), pos);
    }

    template<typename CharT, typename Traits>
    inline
    auto basic_string_view<CharT, Traits>::rfind(CharT *s, size_type pos, size_type count) const -> size_type {
        return find(basic_string_view(s, count), pos);
    }

    template<typename CharT, typename Traits>
    inline
    auto basic_string_view<CharT, Traits>::copy(CharT *dest, size_type count, size_type pos) const -> size_type {
        if (dest == nullptr || count == 0 || pos >= size_) {
            return 0;
        }
        size_type copyLen = count > size_ || count + pos >= size_
                            ? size_ - pos
                            : count;
        const_iterator copyStart = cbegin() + pos;
        const_iterator copyEnd = copyStart + copyLen;
        std::copy(copyStart, copyEnd, dest);
        return copyLen;
    }

    template<typename CharT, typename Traits>
    inline
    int basic_string_view<CharT, Traits>::compare(basic_string_view<CharT, Traits> sv) const noexcept {
        size_type rlen = std::min(size(), sv.size());
        int comp = traits_type::compare(data(), sv.data(), rlen);
        if (comp != 0) {
            return comp;
        }
        return sv.size() - size();
    }

    using string_view = basic_string_view<char>;

    namespace string_view_literals {

        constexpr utils::basic_string_view<char> operator ""_sv(const char *s, size_t length) {
            return {s, length};
        }

        constexpr utils::basic_string_view<wchar_t> operator ""_sv(const wchar_t *s, size_t length) {
            return {s, length};
        }

    } //namespace utils::string_view_literals

} // namespace utils

template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &out, utils::basic_string_view<CharT, Traits> sv) {
    if (!sv.empty()) {
        out.write(sv.c_str(), sv.length());
    }
    return out;
}

#endif //UTILS_STRING_VIEW_HPP
