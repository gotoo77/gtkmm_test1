//
// Created by gduval on 15/04/2022.
//
#include <iostream>
#include <gtkmm/main.h>
#include "utils/log/MyLog.hpp"
#include "utils/dirpath.hpp"
#include "gtkmm/My_CW_main.hpp"
#include "gtkmm/My_CW_pouet.hpp"
#include "gtkmm/My_CW_Form1.hpp"
#include "const_shared.hpp"

using namespace std;

My_CW_main::My_CW_main() : _button_one("_button_onedefault_text !") {
    cout << __FUNCTION__ << " (instanciate _button_one" << endl;

    set_title("My_CW_main demo title !");

    auto exeDir = get_exe_dirpath();
    std::string img_file = exeDir + ".." + dir_sep + "assets" +  dir_sep + "img" +  dir_sep + "logocpp64x64.png";
    std::cout << "img_file : " << img_file << std::endl;
    if (check_file_exist(img_file)) {
        try {
            auto rc = set_icon_from_file(img_file);
            cout << "[OK] set_icon_from_file(" << img_file << ") :[" << boolalpha << rc << "]" << endl;
        }
        catch (const std::exception &e) {
            cerr << "[KO] set_icon_from_file exception :" << e.what() << endl;
        }
    } else {
        cerr << "[KO] img_file :" << img_file << " does not exist." << endl;
    }
    // un label
    _vbox_one.pack_start(_label_one);
    _label_one.show();
    // une zone de saisie
    _vbox_one.pack_start(_entry_one);
    _entry_one.show();
    // connecter les signaux au boutton
    _button_one.signal_clicked().connect(sigc::mem_fun(*this, &My_CW_main::on_btn_clicked_My_CW_Form1));

    _vbox_one.pack_start(_button_one);
    _button_one.show();

    add(_vbox_one);
    _vbox_one.show();
}

void My_CW_main::on_btn_clicked_My_CW_Form1() {
    cout << __FUNCTION__ << endl;
    _label_one.set_text(__FUNCTION__ + _entry_one.get_text() + " !");
    auto form1 = std::make_unique<My_CW_Form1>();
    // Keep the window running
    Gtk::Main::run(*form1);
    // The unique_ptr will automatically clean up the object when it goes out of scope.
}

void My_CW_main::on_btn_clicked_My_CW_pouet() {
    cout << __FUNCTION__ << endl;
    _label_one.set_text(__FUNCTION__ + _entry_one.get_text() + ".");
    // Create a unique_ptr to manage the My_CW_pouet object
    auto nw = std::make_unique<My_CW_pouet>();
    // Keep the window running
    Gtk::Main::run(*nw);
    // The unique_ptr will automatically clean up the object when it goes out of scope.
}

void My_CW_main::on_btn_clic(int id, const Glib::ustring& data) {
    string s = __FUNCTION__;
    s += " > data=[" + data + "] " + _entry_one.get_text() + to_string(id);
    cout << s << endl;
    LOG_D("%s",s.c_str());
    _label_one.set_text(data);
}

My_CW_main::~My_CW_main() {
    cout << __FUNCTION__ << " (delete)" << endl;
}


My_CW_main::My_CW_main(const vector<string>& f_btn_list, const string &f_img_file) {
    cout << __FUNCTION__ << " (instanciate)" << endl;

    set_title("My_CW_main(" + to_string(f_btn_list.size()) + ", " + f_img_file + ")");

    if (check_file_exist(f_img_file)) {
        try {
            auto rc = set_icon_from_file(f_img_file);
            cout << "[OK] set_icon_from_file(" << f_img_file << ") :[" << boolalpha << rc << "]" << endl;
        }
        catch (const exception &e) {
            cerr << "[KO] set_icon_from_file exception :" << e.what() << endl;
        }
    } else {
        cerr << "[KO] img_file :" << f_img_file << " does not exist." << endl;
    }

    _vbox_one.pack_start(_label_one);
    _label_one.show();
    _vbox_one.pack_start(_entry_one);
    _entry_one.show();

    for (auto &lb: f_btn_list) {
        _buttons.emplace_back(lb);
    }

    for (auto &but: _buttons) {
        static int id_but = 0;

        _vbox_one.pack_start(but);
        but.show();

        if (but.get_label() == CONST_SHARED::BTN1) {
            but.signal_clicked().connect(sigc::mem_fun(*this, &My_CW_main::on_btn_clicked_My_CW_Form1));
        } else if (but.get_label() == CONST_SHARED::BTN2) {
            but.signal_clicked().connect(sigc::mem_fun(*this, &My_CW_main::on_btn_clicked_My_CW_pouet));
        } else {
            but.signal_clicked().connect(
                    sigc::bind<int, Glib::ustring>(
                            sigc::mem_fun(
                                    *this,
                                    &My_CW_main::on_btn_clic
                            ),
                            id_but++, but.get_label())
            );
        }

    }
//    _arrow

    add(_vbox_one);
    _vbox_one.show();
}



