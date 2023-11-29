//
// Created by gduval on 15/04/2022.
//

#ifndef GTKMM_MY_CW_POUET_HPP
#define GTKMM_MY_CW_POUET_HPP

#include <gtkmm/hvbox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class My_CW_pouet : public Gtk::Window {
private:


protected:
    void on_btn_click() ;

    // Member variables
    Gtk::Button _button_one;
    Gtk::HBox _layout;


public:
    My_CW_pouet();

    ~My_CW_pouet() override;
};

#endif // GTKMM_MY_CW_POUET_HPP
