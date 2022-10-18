
#include <utils/option/options.hpp>
#include <utils/option/map_entry.hpp>
#include <fstream>
#include <sstream>

namespace utils {
    namespace option {


        command_line_options_description options::add_command_line_options(
                unsigned line_length,
                unsigned min_description_length) {
            if (cmd_options_ == nullptr) {
                cmd_options_ = std::make_unique<boost::program_options::options_description>(
                        line_length, min_description_length);
            }
            return command_line_options_description(this, cmd_options_->add_options());
        }

        command_line_options_description options::add_command_line_options(
                const std::string &caption,
                unsigned line_length,
                unsigned min_description_length) {
            if (cmd_options_ == nullptr) {
                cmd_options_ = std::make_unique<boost::program_options::options_description>(
                        caption, line_length, min_description_length);
            }
            return command_line_options_description(this, cmd_options_->add_options());
        }

        positional_options_description options::add_command_line_positional_options() {
            if (cmd_positional_ == nullptr) {
                cmd_positional_ = std::make_unique<boost::program_options::positional_options_description>();
            }
            if (cmd_hidden_ == nullptr) {
                cmd_hidden_ = std::make_unique<boost::program_options::options_description>();
            }
            return positional_options_description(this, cmd_positional_.get(), cmd_hidden_.get());
        }

        options_description options::add_options(
                unsigned line_length,
                unsigned min_description_length) {
            options_.emplace_back(line_length, min_description_length);
            return options_description(this, options_.back().add_options());
        }

        options_description options::add_options(
                const std::string &caption,
                unsigned line_length,
                unsigned min_description_length) {
            options_.emplace_back(caption, line_length, min_description_length);
            return options_description(this, options_.back().add_options());
        }

        boost::program_options::command_line_parser &
        options::_configure_command_line_parser(boost::program_options::command_line_parser &parser) const {
            if (cmd_positional_) {
                // with positional options
                parser.options(*cmd_hidden_).positional(*cmd_positional_);

            } else if (cmd_options_) {
                // no positional options
                parser.options(*cmd_options_);
            }
            if (allow_unregistered_) {
                parser.allow_unregistered();
            }

            return parser;
        }

        option_list options::parse_config_file(const std::string &file, variables_map &vars) const {
            // check file
            std::ifstream config_file(file);
            if (!config_file) {
                throw boost::program_options::reading_file(file.c_str());
            }

            // remove comment lines with ;
            std::stringstream filtered_file;
            std::string line;
            while (std::getline(config_file, line)) {
                auto semicolon = line.find(';');
                if (semicolon != std::string::npos) {
                    line.erase(semicolon);
                }
                if (!line.empty()) {
                    filtered_file << line << std::endl;
                }
            }

            // boost parse options
            auto parsed = boost::program_options::parse_config_file(
                    filtered_file, file_options_, allow_unregistered_ || allow_map_entries_);
            if (allow_map_entries_) {
                parsed = parse_map_tokens(std::move(parsed), allow_unregistered_);
            }
            boost::program_options::store(parsed, vars);
            boost::program_options::notify(vars);

            return std::move(parsed.options);
        }

        void options::pack() {
            if (cmd_positional_) {
                if (cmd_options_) {
                    // merge the positional equivalent in cmd_hidden with cmd_options
                    cmd_hidden_->add(*cmd_options_);
                }
            }
            for (auto &desc: options_) {
                file_options_.add(desc);
            }
            if (cmd_options_) {
                description_.add(*cmd_options_);
            }
            description_.add(file_options_);
        }

        void options::print_description(std::ostream &out, unsigned width) const {
            description_.print(out, width);
        }

        options_description &
        options_description::operator()(
                const char *name,
                const char *description) {
            ref_(name, description);
            return *this;
        }

        options_description &
        options_description::operator()(
                const char *name,
                const value_semantic *s) {
            ref_(name, s);
            return *this;
        }

        options_description &
        options_description::operator()(
                const char *name,
                const value_semantic *s,
                const char *description) {
            ref_(name, s, description);
            return *this;
        }

        command_line_options_description &
        command_line_options_description::operator()(
                const char *name,
                const char *description) {
            ref_(name, description);
            return *this;
        }


        command_line_options_description &
        command_line_options_description::operator()(
                const char *name,
                const value_semantic *s) {
            ref_(name, s);
            return *this;
        }

        command_line_options_description &
        command_line_options_description::operator()(
                const char *name,
                const value_semantic *s,
                const char *description) {
            ref_(name, s, description);
            return *this;
        }

        positional_options_description &positional_options_description::add(const char *name, int max_count) {
            ref_->add(name, max_count);
            auto semantic = value<std::string>();
            regular_->add_options()(name, semantic);
            return *this;
        }

        positional_options_description &positional_options_description::add(
                const char *name, int max_count,
                value_semantic *semantic) {
            ref_->add(name, max_count);
            regular_->add_options()(name, semantic);
            return *this;
        }

    } // namespace utils::option
} // namespace utils
