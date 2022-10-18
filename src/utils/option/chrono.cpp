
#include <utils/option/chrono.hpp>
#include <regex>
#include <mutex>
#include <boost/program_options.hpp>

std::chrono::seconds _boost_validate_chrono_parse_seconds(const std::string &s) {
    static std::regex chrono_format(R"regex(([0-9]+)(h|min|s))regex");

    std::smatch match_chrono;
    if (std::regex_match(s, match_chrono, chrono_format)) {
        // 1: value
        // 2: unit
        std::string valueString = match_chrono[1].str();
        std::string unitString = match_chrono[2].str();
        long long value = std::strtoll(valueString.c_str(), nullptr, 10);
        if (value < 0) {
            throw boost::program_options::invalid_option_value(s);
        }
        if (unitString == "h") {
            std::chrono::hours hours(value);
            return std::chrono::duration_cast<std::chrono::seconds>(hours);
        }
        if (unitString == "min") {
            std::chrono::minutes minutes(value);
            return std::chrono::duration_cast<std::chrono::seconds>(minutes);
        }
        if (unitString == "s") {
            return std::chrono::seconds(value);
        }
    }
    throw boost::program_options::invalid_option_value(s);
}

template<class ChronoTo>
struct _boost_validate_chrono_safe {
    template<class ChronoFrom>
    static ChronoTo cast(const ChronoFrom value, const std::string &asString) {
        auto castValue = std::chrono::duration_cast<ChronoTo>(value);
        if (value != std::chrono::duration_cast<ChronoFrom>(castValue)) {
            throw boost::program_options::invalid_option_value(asString);
        }
        return castValue;
    }
};

namespace std { //NOLINT NOSONAR define code in std

    void validate(boost::any &v,
                  const vector<string> &values,
                  chrono::seconds *, int) {
        boost::program_options::check_first_occurrence(v);
        const std::string &s = boost::program_options::get_single_string(values);

        if (s == "0") {
            v = chrono::seconds::zero();
            return;
        }

        v = _boost_validate_chrono_parse_seconds(s); // may throw
    }

    void validate(boost::any &v,
                  const vector<string> &values,
                  chrono::minutes *, int) {
        boost::program_options::check_first_occurrence(v);
        const std::string &s = boost::program_options::get_single_string(values);

        if (s == "0") {
            v = chrono::minutes::zero();
            return;
        }

        auto seconds = _boost_validate_chrono_parse_seconds(s); // may throw
        // seconds to minutes may leak a remainder
        v = _boost_validate_chrono_safe<chrono::minutes>::cast(seconds, s); // may throw
    }

    void validate(boost::any &v,
                  const vector<string> &values,
                  chrono::hours *, int) {
        boost::program_options::check_first_occurrence(v);
        const std::string &s = boost::program_options::get_single_string(values);

        if (s == "0") {
            v = chrono::hours::zero();
            return;
        }

        auto seconds = _boost_validate_chrono_parse_seconds(s); // may throw
        // seconds to hours may leak a remainder
        v = _boost_validate_chrono_safe<chrono::hours>::cast(seconds, s); // may throw
    }

}