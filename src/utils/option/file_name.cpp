
#include <utils/option/file_name.hpp>
#include <boost/program_options.hpp>

#include <sys/stat.h>

namespace utils {
    namespace option {

        file_name::file_name(std::string name)
                : basic_holder<std::string>(std::move(name)) {
        }

        file_name::file_name(const char *name)
                : basic_holder<std::string>(name) {
        }

        void validate(boost::any &v,
                      const std::vector <std::string> &values,
                      file_name *, int) {
            namespace po = boost::program_options;

            // check that no previous assignment was made
            po::validators::check_first_occurrence(v);

            // get the first occurence an assert there is no more available
            const std::string &s = po::validators::get_single_string(values);

            struct stat st;
            if (stat(s.c_str(), &st) == 0) { // file exists
                v = boost::any(file_name(s));
                return;
            }

            throw po::validation_error(
                    po::validation_error::invalid_option_value); //NOLINT no nothrow copy-constructible
        }

    } // namespace utils::option
} // namespace utils