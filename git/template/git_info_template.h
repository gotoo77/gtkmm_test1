/*!
 * @file git/template/git_info_template.h
 * @brief generated GIT info (branch, commit...) for main program & dependencies
 * @author %WHO_GEN%
 * @date on %DATE_GEN%
 *
*/

#include <vector>
#include <string>
#include <ostream>
#include <utils/MyLog.hpp.>

#ifndef %PROGNAME%_GIT_INFO_H
#define %PROGNAME%_GIT_INFO_H

/**
 * @brief espace de nom pour le programme courant
 * @ingroup program
 */
namespace program {
/**
 * @brief espace de nom pour les info GIT
 * @ingroup git_info
 */
    namespace git_info {
        /**
         * @struct info GIT
         */
        typedef struct {
            std::string name;
            std::string branch;
            std::string commit;
        } rGitInfo;

        /// enum pour choix de sortie d affichage
        enum class display_out {
            std,
            logger,
            both
        };

        const std::string repo_url =
        %REPO_URL%;
        const rGitInfo program =
        %PROG_GIT_INFO%;
        // for an example:
        // {"TlcFicVADCB","develop","ba13c22bb72109ff325ff3f63572434c83b4c142"};

        const std::vector<rGitInfo> dependencies{
                %DEP_GIT_INFO%
        };
        // for an example:
        //{"lib-common","pouet","zz13c22bb72109ff325ff3f63572434c83b4c142"},
        //{"lib-pouet","pouetpouet","yy13c22bb72109ff325ff3f63572434c83b4c142"},

        std::string get_info_as_str() {
            std::ostringstream oss;
            // build info
            oss << "BUILD on [" << __DATE__ << " " << __TIME__ << "]" << std::endl;
            // program
            oss << " > program GIT INFO [" << program::git_info::program.name
                << "-" << program::git_info::program.branch << "-"
                << program::git_info::program.commit
                << "] -> " << repo_url << program::git_info::program.name << "/commits/"
                << program::git_info::program.commit << std::endl;
            // dependencies
            for (const auto &dep: program::git_info::dependencies) {
                oss << "  - dependency GIT INFO [" << dep.name << "-"
                    << dep.branch << "-" << dep.commit
                    << "] -> " << repo_url << dep.name << "/commits/"
                    << dep.commit << std::endl;
            }
            return oss.str();
        }

        void print_std() {
            // build info
            std::cout << get_info_as_str();
        }

        void print_logger() {
            // build info
            LOG_I("BUILD on [%s %s]", __DATE__, __TIME__);
            // program
            LOG_I("> program GIT INFO name[%s]", program::git_info::program.name.c_str());
            LOG_I("> program GIT INFO branch[%s]", program::git_info::program.branch.c_str());
            LOG_I("> program GIT INFO commit[%s]", program::git_info::program.commit.c_str());
            // dependencies
            for (const auto &dep: program::git_info::dependencies) {
                LOG_I("   - dependency GIT INFO [%s]", dep.name.c_str());
                LOG_I("   - dependency GIT INFO branch[%s]", dep.branch.c_str());
                LOG_I("   - dependency GIT INFO commit[%s]", dep.commit.c_str());
            }
        }

        void print(display_out out) {
            using disp = display_out;
            switch (out) {
                case disp::std:
                    print_std();
                    break;

                case disp::both:
                    print_logger();
                    print_std();
                    break;

                case disp::logger:
                default:
                    print_logger();
                    break;
            }
        }

    } // git_info
} // program

#endif // %PROGNAME%_GIT_INFO_H