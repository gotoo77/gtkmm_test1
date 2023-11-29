//
// Created by gduval on 15/04/2022.
//

#ifndef GTKMM_MY_CW_FORM1_HPP
#define GTKMM_MY_CW_FORM1_HPP

#include <gtkmm/hvbox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>

#include <curl/curl.h>


class My_CW_Form1 : public Gtk::Window {

protected:
    CURL *      _pCurl  {};
    std::string _url    = "https://example.com";  // Replace with your desired URL
    std::string _data;
    Gtk::Entry  _entry;
    Gtk::Button _button;
//    Gtk::VBox   _layout;

    Gtk::ScrolledWindow             _scrolled_window;
    Gtk::TextView                   _text_view;
    Glib::RefPtr<Gtk::TextBuffer>   _text_buffer;

    void on_button_clicked() ;
    bool curl_init();
    bool curl_send();

public:
    My_CW_Form1();

    ~My_CW_Form1() override;
};

#endif // GTKMM_MY_CW_FORM1_HPP
