//
// Created by gduval on 15/04/2022.
//
#include <iostream>
#include "include/gtkmm/My_CW_pouet.hpp"

using namespace std;

My_CW_pouet::My_CW_pouet() : _button_one("pouet !") {
    cout << __FUNCTION__ << " (instanciate)" << endl;

    set_title(__FUNCTION__);
    set_default_size(800, 600);

    // Create and connect button click signal handler
    _button_one.set_label("pouet New Window");
    _button_one.signal_clicked().connect(sigc::mem_fun(*this, &My_CW_pouet::on_btn_click));

    // Add button to the layout
    _layout.pack_start(_button_one, Gtk::PACK_SHRINK);

    // Set the layout for the main window
    add(_layout);

    show_all();
}

void My_CW_pouet::on_btn_click() {
    cout << "My_CW_pouet::on_btn_click" << endl;
}


My_CW_pouet::~My_CW_pouet() {
    cout << __FUNCTION__ << " (delete)" << endl;
}





