#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <Global.h>
#include "include/gtkmm/My_CW_main.hpp"

#include "external/Version.h"
#include "include/utils/log/MyLog.hpp"
#include <utils/AutoClose.hpp>
#include "include/utils/config/read_val.hpp"
#include <fstream>
#include "Global.h"
#include "utils/dirpath.hpp"
#include "string_gen.hpp"
#include "myAppInfo.hpp"
#include "cond_print.hpp"
#include "const_shared.hpp"
#include <unistd.h>

#ifdef _WIN32

#include <win_ports/syslog.h>

#else
#include <syslog.h>
#endif

Globals G; // NOLINT : oui on est des fous on utilise des variables globales :)
bool read_conf(const std::string &f_conf_path);

void open_trace();

int demo1(int argc, char **argv) {
    std::cout << "demo1" << std::endl;

    Gtk::Main app(argc, argv);
    Gtk::Window w;
//    Gtk::VBox vb;
    Gtk::Box my_box1;
//    w.add(vb);
    w.add(my_box1);

    Gtk::Label l;
    l.set_label("mon_label1");
    my_box1.pack_start(l);
    l.show();

    Gtk::Entry e;
    my_box1.pack_start(e);
    e.show();

    Gtk::Button my_btn1("my_btn1 button!");
    my_box1.pack_start(my_btn1);
    my_btn1.show();
    my_box1.show();

    Gtk::Main::run(w);
    return 0;
}

int demo_gtkapp() {
    auto app = Gtk::Application::create();
    auto *button = new Gtk::Button("Quit");
    Gtk::Window main_window;
    button->signal_clicked().connect(sigc::mem_fun(&main_window, &Gtk::Window::close));

    main_window.set_default_size(400, 50);

    Gtk::Box my_box1;
    main_window.add(my_box1);

    Gtk::Label my_label1;
    my_label1.set_label("my label");
    my_box1.pack_start(my_label1);
    my_label1.show();

    Gtk::Entry my_entry1;
    my_box1.pack_start(my_entry1);
    my_entry1.show();

    // add box to main window
    main_window.add(my_box1);

    my_box1.pack_start(*button);
    button->show();
    my_box1.show();

    //    main_window.add(*button);
    //    main_window.show_all(); //for some widgets (I don't remember which) show() is not enough

    return app->run(main_window);
}

int demo_CustomWindow(int argc, char **argv) {
    Gtk::Main app(argc, argv);


    std::string exeDir = get_exe_dirpath();
    std::cout << "exeDir : " << exeDir << std::endl;
    //std::string img = "/media/sf_VM_linux/tests/gtkmm_test1/assets/img/logocpp64x64.png";
    std::string img = exeDir + ".." + dir_sep + "assets" + dir_sep + "img" + dir_sep + "logocpp64x64.png";

    std::vector<std::string> btn_list = {CONST_SHARED::BTN1, CONST_SHARED::BTN2,
                                         "trotro", "yolo", "pouet",
                                         "gros bouton badass", "3", "tata monique"};

    My_CW_main main_w(btn_list, img);
    Gtk::Main::run(main_w);
    return 0;
}

void open_trace() {
    myAppInfo prog(PROGRAM_NAME, PROGRAM_VERSION);
#ifdef _WIN32
    init_syslog("localhost");
#endif
    // Ouverture de la trace
    std::string progId = prog.getFullName();
    utils::AutoClose autoClose;
    autoClose.acquire(&Log::init, Log::Init{
            .program = progId.c_str(),
            .level = G.cfg_syslog.level,
            .facility = G.cfg_syslog.facility,
            .console = G.cfg_syslog.console
    });
    LOG_N("Starting %s", progId.c_str());
}

bool read_conf(const std::string &f_conf_path) {
    std::ifstream f;
    std::string line;
    std::string key;
    std::string value;
    G.verbose = false;
    G.cfg_syslog.console = false;
    G.cfg_syslog.facility = utils::syslog_facility::local_1;
    G.cfg_syslog.level = utils::log_level::debug;

    f.open(f_conf_path.c_str());
    if (!f.is_open()) {
        std::cerr << "[" << __FUNCTION__ << "] failed reading conf '" << f_conf_path.c_str() << "' - open failed";
        return false;
    }

    while (getline(f, line)) {
        if (getKeyVal(line, '=', key, value)) {
            if (key == "LogLevel") {
                if (
                        (value == "LOG_EMERG") ||
                        (value == "LOG_ALERT") ||
                        (value == "LOG_CRIT") ||
                        (value == "LOG_ERR")
                        ) {
                    G.cfg_syslog.level = utils::log_level::error;
                } else if (value == "LOG_WARNING")
                    G.cfg_syslog.level = utils::log_level::warning;
                else if (value == "LOG_NOTICE")
                    G.cfg_syslog.level = utils::log_level::notice;
                else if (value == "LOG_INFO")
                    G.cfg_syslog.level = utils::log_level::info;
                else
                    G.cfg_syslog.level = utils::log_level::debug;
            } else if (key == "LogFacility") {
                if (value == "LOCAL0")
                    G.cfg_syslog.facility = utils::syslog_facility::local_0;
                else if (value == "LOCAL2")
                    G.cfg_syslog.facility = utils::syslog_facility::local_2;
                else if (value == "LOCAL3")
                    G.cfg_syslog.facility = utils::syslog_facility::local_3;
                else if (value == "LOCAL4")
                    G.cfg_syslog.facility = utils::syslog_facility::local_4;
                else if (value == "LOCAL5")
                    G.cfg_syslog.facility = utils::syslog_facility::local_5;
                else if (value == "LOCAL7")
                    G.cfg_syslog.facility = utils::syslog_facility::local_7;
                else
                    G.cfg_syslog.facility = utils::syslog_facility::local_1;
            } else if (key == "console") {
                if (value == "1" || value == "true")
                    G.cfg_syslog.console = true;
                else
                    G.cfg_syslog.console = false;
            } else if (key == "verbose") {
                if (value == "1" || value == "true") G.verbose = true;
                else G.verbose = false;
            }
        }
    }
    return true;
}

int main(int argc, char **argv) {

    open_trace();
    std::ostringstream log;
    log << " *** Welcome to " << PROGRAM_NAME << " v" << PROGRAM_VERSION << " ***";
    LOG_D("%s", log.str().c_str());

    // read conf and set log options
    auto exeDir = get_exe_dirpath();
    std::string conf_path = exeDir + ".." + dir_sep + "conf" + dir_sep + "gtkmm.ini";
    std::cout << "conf_path : " << conf_path << std::endl;
    auto conf_read = read_conf(conf_path);
    if (!conf_read) {
        std::cerr << "conf has not been read " << std::endl;
    }

    if (argc >= 2) {
        auto arg1 = std::string(argv[1]);
        if (arg1 == "demo1") {
            log.str("");
            log.clear(); // Clear log contents
            log << "running demo1() entry point";
            LOG_D("%s", log.str().c_str());
            return demo1(argc, argv);
        } else {
            log.str("");
            log.clear(); // Clear log contents
            log << "running demo_CustomWindow() entry point";
            LOG_D("%s", log.str().c_str());
            return demo_CustomWindow(argc, argv);
        }
    } else {
        log.str("");
        log.clear(); // Clear log contents
        log << "running demo_gtkapp() entry point";
        LOG_D("%s", log.str().c_str());
        demo_gtkapp();
    }

    log.str("");
    log.clear(); // Clear log contents
    log << "closing log - exiting program";
    LOG_N("%s", log.str().c_str());
    closelog();
    return 0;
}
