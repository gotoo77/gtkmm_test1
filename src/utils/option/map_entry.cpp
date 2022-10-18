
#include <boost/any.hpp>
#include <boost/program_options/parsers.hpp>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include <locale>

namespace std { // NOLINT NOSONAR required by boost::program_options

    void
    validate(boost::any &value_store, const std::vector<std::basic_string<char> > &values,
             std::map<std::string, std::string> *,
             long int) {
        using map_type = std::map<std::string, std::string>;
        static std::regex keyValueFormat(R"regex(\[(.*)\]=(.*))regex");
        for (auto &line: values) {
            std::smatch matchKeyValue;
            if (std::regex_match(line, matchKeyValue, keyValueFormat)) {
                // 1: key
                // 2: value
                std::string key = matchKeyValue[1].str();
                std::string value = matchKeyValue[2].str();

                map_type entries;
                entries.emplace(std::move(key), std::move(value));
                if (!value_store.empty()) {
                    map_type oldEntries = boost::any_cast<map_type>(value_store);
                    entries.insert(oldEntries.begin(), oldEntries.end());
                }
                value_store = entries;
            }
        }
    }

} // namespace std

namespace utils {
    namespace option {

        template<typename CharT = char>
        std::basic_string<char>
        parse_map_entry_lazy_string(const std::basic_string<CharT> &text);

        template<>
        std::basic_string<char>
        parse_map_entry_lazy_string(const std::basic_string<char> &text) {
            return text;
        }

        template<>
        std::basic_string<char>
        parse_map_entry_lazy_string(const std::basic_string<wchar_t> &text) {
            static const std::locale locale("");

            using converter_t = std::codecvt<wchar_t, char, std::mbstate_t>;
            const auto &converter = std::use_facet<converter_t>(locale);

            std::vector<char> to(text.length() * converter.max_length());

            std::mbstate_t state;
            const wchar_t *from_next;
            char *to_next;
            const converter_t::result result = converter.out(state, text.data(), text.data() + text.length(), from_next,
                                                             &to[0], &to[0] + to.size(), to_next);

            bool failure = result != converter_t::ok && result != converter_t::noconv;
            std::string s(&to[0], to_next);
            if (failure) {
                s += "...";
            }
            return s;
        }

        template<typename CharT = char>
        std::pair<std::basic_string<char>, std::vector<std::basic_string<CharT> >>
        parse_map_entry(const std::vector<std::basic_string<CharT>> &tokens);

        template<>
        std::pair<std::basic_string<char>, std::vector<std::basic_string<char> >>
        parse_map_entry<char>(const std::vector<std::basic_string<char>> &tokens) {
            static const std::regex entryFormat(R"regex(^(.*)(\[.*\])$)regex");
            if (tokens.empty()) {
                return {};
            }
            std::smatch matchEntry;
            if (std::regex_match(tokens[0], matchEntry, entryFormat)) {
                // 1: key
                // 2: entry key
                std::string key = matchEntry[1].str();
                std::string entryKey = matchEntry[2].str();
                std::vector<std::basic_string<char>> entryValue;
                for (auto it = std::next(tokens.begin()); it != tokens.end(); ++it) {
                    entryValue.emplace_back(entryKey + '=' + *it);
                }
                return std::make_pair(key + "[]", std::move(entryValue));
            }
            return {};
        }

        template<>
        std::pair<std::basic_string<char>, std::vector<std::basic_string<wchar_t> >>
        parse_map_entry<wchar_t>(const std::vector<std::basic_string<wchar_t>> &tokens) {
            static const std::wregex entryFormat(LR"regex(^(.*)(\[.*\])$)regex");
            if (tokens.empty()) {
                return {};
            }
            std::wsmatch matchEntry;
            if (std::regex_match(tokens[0], matchEntry, entryFormat)) {
                // 1: key
                // 2: entry key
                std::string key = parse_map_entry_lazy_string(matchEntry[1].str());
                std::wstring entryKey = matchEntry[2].str();
                std::vector<std::basic_string<wchar_t>> entryValue;
                for (auto it = std::next(tokens.begin()); it != tokens.end(); ++it) {
                    entryValue.emplace_back(entryKey + L'=' + *it);
                }
                return std::make_pair(key + "[]", std::move(entryValue));
            }
            return {};
        }

        template<typename CharT = char>
        boost::program_options::basic_parsed_options <CharT>
        parse_map_tokens(boost::program_options::basic_parsed_options <CharT> &&parsed, bool allow_unregistered) {
            namespace po = boost::program_options;
            auto &options = parsed.description->options();
            for (auto &opt: parsed.options) {
                if (opt.unregistered && !opt.original_tokens.empty()) {
                    auto entry = parse_map_entry(opt.original_tokens);
                    if (!entry.first.empty()) {
                        auto found = std::find_if(
                                options.begin(), options.end(),
                                [&entry](boost::shared_ptr<const po::option_description> descr) {
                                    auto result = descr->match(entry.first, false, false, false);
                                    return result == po::option_description::full_match;
                                });
                        if (found != options.end()) {
                            opt.string_key = entry.first;
                            opt.value = entry.second;
                            opt.unregistered = false;
                        }
                    }
                }
            }

            if (!allow_unregistered) {
                std::vector<std::basic_string<CharT>> unrecognized = po::collect_unrecognized(parsed.options,
                                                                                              po::exclude_positional);
                if (!unrecognized.empty()) {
                    throw po::unknown_option(parse_map_entry_lazy_string(unrecognized[0]));
                }
            }

            return std::move(parsed);
        }

        template
        boost::program_options::basic_parsed_options<char>
        parse_map_tokens<char>(boost::program_options::basic_parsed_options<char> &&parsed, bool allow_unregistered);

        template
        boost::program_options::basic_parsed_options<wchar_t>
        parse_map_tokens<wchar_t>(boost::program_options::basic_parsed_options<wchar_t> &&parsed,
                                  bool allow_unregistered);

    } // namespace utils::option
} // namespace utils
