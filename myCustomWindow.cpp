//
// Created by gduval on 15/04/2022.
//
#include <iostream>
#include "myCustomWindow.h"

using namespace std;

myCustomWindow::myCustomWindow() : button_one_("button_one_default_text !") {
    set_title("myCustomWindow demo title !");
    string img_file = "/media/sf_VM_linux/tests/gtkmm_test1/assets/img/logocpp64x64.png";
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

    vbox_one_.pack_start(label_one_);
    label_one_.show();
    vbox_one_.pack_start(entry_one_);
    entry_one_.show();
    // connecter les signaux au boutton
    button_one_.signal_clicked().connect(sigc::mem_fun(*this, &myCustomWindow::on_btn_clicked1));

//    button_one_.signal_clicked().connect(SigC::slot(*this, &myCustomWindow::on_button_clicked));
//    b.signal_clicked().connect(sigc::slot<void(int) , &myCustomWindow::on_button_clicked>);

    vbox_one_.pack_start(button_one_);
    button_one_.show();

    add(vbox_one_);
    vbox_one_.show();
}

void myCustomWindow::on_btn_clicked1() {
    cout << __FUNCTION__ << endl;
    label_one_.set_text(__FUNCTION__ + entry_one_.get_text() + " !");
}

void myCustomWindow::on_btn_clicked2() {
    cout << __FUNCTION__ << endl;
    label_one_.set_text(__FUNCTION__ + entry_one_.get_text() + ".");
}

void myCustomWindow::on_btn_clic(int id, Glib::ustring data) {
    std::string s = __FUNCTION__;
    s += " > data=[" + data + "] " + entry_one_.get_text() + std::to_string(id);
    cout << s << endl;
    label_one_.set_text(data);
}

myCustomWindow::~myCustomWindow() {
    cout << "~myCustomWindow " << this->label_one_.get_text() << endl;
}

inline bool myCustomWindow::check_file_exist(const std::string &name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

myCustomWindow::myCustomWindow(std::vector<std::string> f_btn_list, const std::string &f_img_file) {
    set_title("myCustomWindow(" + std::to_string(f_btn_list.size()) + ", " + f_img_file + ")");

    if (check_file_exist(f_img_file)) {
        try {
            auto rc = set_icon_from_file(f_img_file);
            cout << "[OK] set_icon_from_file(" << f_img_file << ") :[" << boolalpha << rc << "]" << endl;
        }
        catch (const std::exception &e) {
            cerr << "[KO] set_icon_from_file exception :" << e.what() << endl;
        }
    } else {
        cerr << "[KO] img_file :" << f_img_file << " does not exist." << endl;
    }

    vbox_one_.pack_start(label_one_);
    label_one_.show();
    vbox_one_.pack_start(entry_one_);
    entry_one_.show();

    for (auto &lb: f_btn_list) {
        buttons_.emplace_back(lb);
    }

    for (auto &but: buttons_) {
        static int id_but = 0;

        vbox_one_.pack_start(but);
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

    add(vbox_one_);
    vbox_one_.show();
}



