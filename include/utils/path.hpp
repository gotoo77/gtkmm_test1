
#ifndef UTILS_URI_HPP
#define UTILS_URI_HPP

#include <map>
#include <vector>
#include <string>
#include <utils/string_view.hpp>

namespace utils {

/**
 * Handles a simple URL-path with an optional query string. Supports default container iteration for path components.
 */
    class path final {
    public:
        /**
         * the type of paths components
         */
        using component_type = std::string;
        /**
         * the type of query string keys
         */
        using key_type = std::string;
        /**
         * The type of read-write iterators for path components
         */
        using component_iterator = std::vector<component_type>::iterator;
        /**
         * The type of read-only iterators for path components
         */
        using component_const_iterator = std::vector<component_type>::const_iterator;
        /**
         * The type of read-write iterators for query string key-pairs.
         */
        using query_iterator = std::map<key_type, component_type>::iterator;
        /**
         * The type of read-only iterators for query-sting key-pairs.
         */
        using query_const_iterator = std::map<key_type, component_type>::const_iterator;

        /**
         * @brief The default path separator
         *
         * The default path separator is used to break down paths from strings and to serialize paths to strings.
         */
        static constexpr char DEFAULT_SEPARATOR = '/';

    private:
        std::vector<std::string> components_;
        std::map<std::string, std::string> query_;

    public:
        path() = default;

        path(const path &other) = default;

        path(path &&other) = default;

        ~path() = default;

        path &operator=(const path &other) = default;

        path &operator=(path &&other) = default;

        /**
         * @brief Count the number of path components
         *
         * @return The number of path components.
         */
        std::size_t count() const;

        /**
         * @brief Count the number of query string elements.
         *
         * @return The number of query string key pairs
         */
        std::size_t query_count() const;

        /**
         * @brief Queries for the presence of a key in the query string.
         *
         * @param key the key to test.
         * @return true if the key is present in the query string.
         */
        bool query_has(const key_type &key) const;

        /**
         * @brief Get a path component by index.
         *
         * @param index the index of the path component to get. Indexes starts at 0.
         * @return The path components at the specified index.
         */
        const component_type &operator[](std::size_t index) const;

        /**
         * @brief Get a query string value for a key
         *
         * @param key The key to get the value for
         * @return The value associated with the key, if the key is present.
         * @throws std::out_of_range if the key is not in the query string.
         */
        const component_type &operator[](const key_type &key) const;

        /**
         * @brief Iterator to the first path component. Read-only.
         *
         * Iterating path components guaranties a stable ordering by position.
         *
         * @return The iterator to the first path component, or end() if the path is empty.
         */
        component_const_iterator begin() const;

        /**
         * @brief Iterator to the end of path. Read-only.
         *
         * The end of path is the position past the last path component.
         *
         * @return The end of path iterator.
         */
        component_const_iterator end() const;

        /**
         * @brief Iterator to the first query string key-pair.
         *
         * Iterating query string key-pairs does not guaranties any particular order between keys or values.
         *
         * @return
         */
        query_const_iterator query_begin() const;

        /**
         * @brief Iterator the end of query string.
         *
         * The end of query string is the position past the last key-pair
         *
         * @return  The end of query string iterator
         */
        query_const_iterator query_end() const;

        /**
         * @brief Tests that two paths are equal.
         *
         * Two paths are considered equal if:
         * - they have the same size
         * - each element from one is equal to the element from the other at the same position
         * - both query strings contains the same keys with the same associated values
         *
         * @param other The path to compare equality with
         * @return true if the paths are equal
         */
        bool operator==(const path &other) const;

        /**
         * @see begins_with
         *
         * @param other
         * @return
         */
        bool operator<=(const path &other) const;

        /**
         * @brief Check whether this path begins with the provided path
         *
         * This is the case iff:
         * - the other path has at most the size of this path
         * - each element of the other path has the same value with the element of this path at the same position
         *
         * The query string is ignored.
         *
         * @param other the other path to compare with
         * @return true if the other path is the beginning of this path
         */
        bool begins_with(const path &other) const;

        /**
         * @brief creates a representation of this path relative to another path.
         *
         * This is especially useful with the other path is a beginning of this path. In that case the local path would be
         * would be the elements of this path without the beginning (the elements of the other)
         *
         * More generally the process cof constructing the local path is the following:
         * - determining the common root R between two paths
         * - using N = (Other - R) as the base of the new path
         * - appending a parent element '..' to N for each element in (Other - R)
         * - appending (this - R) to N
         *
         * The query string for the new path is the one of this path.
         *
         * Examples:
         * @code{.cpp}
         * // would be three/four
         * path::break_down("/one/two/three/four").local_to(path::break_down("one/two"));
         *
         * // would be five/six/../../three/four
         * path::break_down("/one/two/three/four").local_to(path::break_down("one/two/five/six"));
         * @endcode
         */
        path local_to(const path &other) const;

        /**
         * @brief Creates a string representation of this path with the query string.
         *
         * @param separator The path separator to use.
         * @return the string full representation with the specified separator.
         */
        std::string to_string(char separator = DEFAULT_SEPARATOR) const;

        /**
         * @brief Creates a representation of this path without the query string.
         *
         * @param separator the path separator to use
         * @return The path only representation
         */
        std::string path_string(char separator = DEFAULT_SEPARATOR) const;

        /**
         * @brief the query string representation
         * @return the query string only representation.
         */
        std::string query_string() const;

        /**
         * @brief appends a component ot the path.
         *
         * Appends a component to the path. Prevents the following values from being added:
         * - empty string
         * - current path component ('.')
         *
         * @param component the path component to add
         */
        void append(const std::string &component);

        /**
         * @brief add a query string parameter
         *
         * @param key the key
         * @param value the corresponding value.
         */
        void append_query(const std::string &key, const std::string &value);

        /**
         * @brief static factory to create paths from their string value
         *
         * @param path_string The string to create a path from
         * @param separator The path separator
         * @return The path representation
         */
        static path break_down(utils::string_view path_string, char separator = DEFAULT_SEPARATOR);
    };

} // namespace utils

#endif //UTILS_URI_HPP
