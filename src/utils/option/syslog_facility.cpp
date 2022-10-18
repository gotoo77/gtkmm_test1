
#include <utils/option/syslog_facility.hpp>
#include <boost/program_options.hpp>
#include <regex>

namespace utils {

    void validate(boost::any &v,
                  const std::vector<std::string> &values,
                  syslog_facility *, int) {
        static const std::regex syslog_facility_check(R"regex(USER|LOCAL_[0-7])regex");

        namespace po = boost::program_options;

        // check that no previous assignment was made
        po::validators::check_first_occurrence(v);

        // get the first occurence an assert there is no more available
        const std::string &s = po::validators::get_single_string(values);

        // empty is allowed
        if (s.empty()) {
            v = boost::any(syslog_facility::none);
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
        if (std::regex_match(s, match, syslog_facility_check)) {
            const std::string &m = match[0];
            if (m == "USER") {
                v = boost::any(syslog_facility::user);
            } else if (m == "LOCAL_0") {
                v = boost::any(syslog_facility::local_0);
            } else if (m == "LOCAL_1") {
                v = boost::any(syslog_facility::local_1);
            } else if (m == "LOCAL_2") {
                v = boost::any(syslog_facility::local_2);
            } else if (m == "LOCAL_3") {
                v = boost::any(syslog_facility::local_3);
            } else if (m == "LOCAL_4") {
                v = boost::any(syslog_facility::local_4);
            } else if (m == "LOCAL_5") {
                v = boost::any(syslog_facility::local_5);
            } else if (m == "LOCAL_6") {
                v = boost::any(syslog_facility::local_6);
            } else if (m == "LOCAL_7") {
                v = boost::any(syslog_facility::local_7);
            } else {
                throw po::validation_error(po::validation_error::invalid_option_value, s);
            }

            return;
        }

        throw po::validation_error(po::validation_error::invalid_option_value, s);
    }

} // namespace utils
