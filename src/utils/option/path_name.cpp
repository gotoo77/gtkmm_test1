
#include <utils/option/path_name.hpp>
#include <boost/program_options.hpp>
#include <regex>

namespace utils {
    namespace option {


        path_name::path_name(std::string name)
                : basic_holder<std::string>(std::move(name)) {
        }

        path_name::path_name(const char *name)
                : basic_holder<std::string>(name) {
        }

        void validate(boost::any &v,
                      const std::vector<std::string> &values,
                      path_name *, int) {
            static std::regex path_component(R"regex([-+_a-zA-Z0-9 ./%]+)regex");

            namespace po = boost::program_options;

            // check that no previous assignment was made
            po::validators::check_first_occurrence(v);

            // get the first occurence an assert there is no more available
            const std::string &s = po::validators::get_single_string(values);

            // validate
            std::smatch match;
            if (std::regex_match(s, match, path_component)) {
                v = boost::any(path_name(match[0]));
                return;
            }
            // notify error as requested by the validator protocol
            throw po::validation_error(
                    po::validation_error::invalid_option_value); //NOLINT no nothrow copy-constructible
        }

    } // namespace utils::option
} // namespace utils
