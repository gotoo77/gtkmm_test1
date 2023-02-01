//
// Created by gduval on 15/04/2022.
//
#include <iostream>
#include "include/gtkmm/myCustomWindow.hpp"

using namespace std;

myCustomWindow::myCustomWindow() : _button_one("_button_onedefault_text !") {
    set_title("myCustomWindow demo title !");
//    string img_file = "/media/sf_VM_linux/tests/gtkmm_test1/assets/img/logocpp64x64.png";
    string img_file = "assets/img/logocpp64x64.png";
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
    _button_one.signal_clicked().connect(sigc::mem_fun(*this, &myCustomWindow::on_btn_clicked1));

    _vbox_one.pack_start(_button_one);
    _button_one.show();

    add(_vbox_one);
    _vbox_one.show();
}

void myCustomWindow::on_btn_clicked1() {
    cout << __FUNCTION__ << endl;
    _label_one.set_text(__FUNCTION__ + _entry_one.get_text() + " !");
}

void myCustomWindow::on_btn_clicked2() {
    cout << __FUNCTION__ << endl;
    _label_one.set_text(__FUNCTION__ + _entry_one.get_text() + ".");
}

void myCustomWindow::on_btn_clic(int id, const Glib::ustring& data) {
    string s = __FUNCTION__;
    s += " > data=[" + data + "] " + _entry_one.get_text() + to_string(id);
    cout << s << endl;
    _label_one.set_text(data);
}

myCustomWindow::~myCustomWindow() {
    cout << "~myCustomWindow (delete)" << endl;
}


myCustomWindow::myCustomWindow(const vector<string>& f_btn_list, const string &f_img_file) {
    cout << "myCustomWindow instanciate " << endl;

    set_title("myCustomWindow(" + to_string(f_btn_list.size()) + ", " + f_img_file + ")");

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

        if (but.get_label() == "1") {
            but.signal_clicked().connect(sigc::mem_fun(*this, &myCustomWindow::on_btn_clicked1));
        } else if (but.get_label() == "2") {
            but.signal_clicked().connect(sigc::mem_fun(*this, &myCustomWindow::on_btn_clicked2));
        } else {
            but.signal_clicked().connect(
                    sigc::bind<int, Glib::ustring>(
                            sigc::mem_fun(
                                    *this,
                                    &myCustomWindow::on_btn_clic
                            ),
                            id_but++, but.get_label())
            );
        }

    }

    add(_vbox_one);
    _vbox_one.show();
}



