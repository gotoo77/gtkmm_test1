
#include <utils/option/log_level.hpp>
#include <boost/program_options.hpp>
#include <regex>

namespace utils {

    void validate(boost::any &v,
                  const std::vector<std::string> &values,
                  log_level *, int) {
        static const std::regex log_level_check(R"regex(ERROR|WARNING|NOTICE|INFO|DEBUG)regex");

        namespace po = boost::program_options;

        // check that no previous assignment was made
        po::validators::check_first_occurrence(v);

        // get the first occurence an assert there is no more available
        const std::string &s = po::validators::get_single_string(values);

        // empty is allowed
        if (s.empty()) {
            v = boost::any(log_level::none);
            return;
        }

        // accept none case insensitive
        std::string lower = s;
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (lower == "none") {
            v = boost::any(log_level::none);
            return;
        }

        // validate
        std::smatch match;
        if (std::regex_match(s, match, log_level_check)) {
            const std::string &m = match[0];

            if (m == "ERROR") {
                v = boost::any(log_level::error);
            } else if (m == "WARNING") {
                v = boost::any(log_level::warning);
            } else if (m == "NOTICE") {
                v = boost::any(log_level::notice);
            } else if (m == "INFO") {
                v = boost::any(log_level::info);
            } else if (m == "DEBUG") {
                v = boost::any(log_level::debug);
            } else {
                throw po::validation_error(po::validation_error::invalid_option_value,
                                           s); //NOLINT no nothrow copy-constructible
            }
            return;
        }

        throw po::validation_error(po::validation_error::invalid_option_value,
                                   s); //NOLINT no nothrow copy-constructible
    }

} // namespace utils
