
#include "utils/path.hpp"
#include "utils/type_traits.hpp"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <iterator>
#include <sstream>

namespace utils {

    std::size_t path::count() const {
        return components_.size();
    }

    std::size_t path::query_count() const {
        return query_.size();
    }

    bool path::query_has(const key_type &key) const {
        return query_.count(key) > 0;
    }

    const path::component_type &path::operator[](std::size_t index) const {
        return components_[index];
    }

    const path::component_type &path::operator[](const key_type &key) const {
        return query_.at(key);
    }

    path::component_const_iterator path::begin() const {
        return components_.begin();
    }

    path::component_const_iterator path::end() const {
        return components_.end();
    }

    path::query_const_iterator path::query_begin() const {
        return query_.begin();
    }

    path::query_const_iterator path::query_end() const {
        return query_.end();
    }

    bool path::operator==(const path &other) const {
        if (components_.size() != other.components_.size()) {
            return false;
        }
        if (!std::equal(components_.begin(), components_.end(), other.components_.end())) {
            return false;
        }
        return query_ == other.query_;
    }

    bool path::operator<=(const path &other) const {
        return begins_with(other);
    }

    bool path::begins_with(const path &other) const {
        if (components_.size() < other.components_.size()) {
            return false;
        }
        if (!std::equal(other.components_.begin(), other.components_.end(), components_.begin())) {
            return false;
        }
        return other.query_.empty() || query_ == other.query_;
    }


    path path::local_to(const path &other) const {
        // ignores protocol+host+port+query of other
        size_t last_equal = 0;
        for (size_t i = 0, n = std::min(components_.size(), other.components_.size()); i < n; ++i) {
            if (components_[i] == other.components_[i]) {
                last_equal = i + 1;
                continue;
            }
            break;
        }

        path local;
        local.query_ = query_;

        // skip the common components and count the remaining from other
        typename std::make_signed<std::size_t>::type parents =
                other.components_.end() - (other.components_.begin() + last_equal);
        // put parent tokens
        if (parents > 0) {
            local.components_.insert(local.components_.end(), static_cast<std::size_t>(parents), "..");
        }
        // put the remaining of this
        if (last_equal < components_.size()) {
            local.components_.insert(
                    local.components_.end(), components_.begin() + last_equal, components_.end());
        }

        return local;
    }

    std::string path::to_string(char separator) const {
        std::string s = path_string(separator);

        if (!query_.empty()) {
            s += "?";
            s += query_string();
        }
        return s;
    }

    std::string path::path_string(char separator) const {
        if (components_.empty()) {
            return {1, separator};
        }
        std::string s(1, separator);
        s += boost::join(components_, std::string(1, separator));
        return s;
    }

    std::string path::query_string() const {
        if (query_.empty()) {
            return "";
        }
        std::ostringstream qry_elements;
        std::transform(query_.begin(), query_.end(),
                       std::ostream_iterator<std::string>(qry_elements, "&"),
                       [](const std::pair<key_type, component_type> &entry) -> std::string {
                           std::string second = entry.second;
                           URLEncode(second);
                           return entry.first + "=" + second;
                       });
        std::string res = qry_elements.str();
        res.erase(res.size() - 1, 1);
        return res;
    }

    path path::break_down(utils::string_view path_string, char separator) {
        path to_path;

        std::size_t query_start = path_string.find('?');

        auto path_loc = path_string.substr(0, query_start);
        boost::split(to_path.components_, path_loc,
                     [separator](char c) -> bool {
                         return c == separator;
                     });

        // cleanup, empty and "." elements do no provide path progress
        if (!to_path.components_.empty()) {
            auto remove_it = std::remove_if(
                    to_path.components_.begin(),
                    to_path.components_.end(),
                    [](const std::string &s) { return s.empty() || s == "."; });

            to_path.components_.erase(remove_it, to_path.components_.end());
        }

        if (query_start != std::string::npos) {
            auto query_string = path_string.substr(query_start + 1);
            std::vector<std::string> query_elements;
            boost::split(query_elements, query_string,
                         [](char c) -> bool {
                             return c == '&';
                         });
            for (auto &e: query_elements) {
                std::size_t mark = e.find('=');
                if (mark != 0) {
                    std::string param = (mark == std::string::npos) ? "" : e.substr(mark + 1);
                    URLDecode(param);
                    to_path.query_[e.substr(0, mark)] = param;
                }
            }
        }

        return to_path;
    }

    void path::append(const std::string &component) {
        if (!component.empty() || component != ".") {
            components_.push_back(component);
        }
    }

    void path::append_query(const std::string &key, const std::string &value) {
        query_[key] = value;
    }

} // namespace utils
