//
// Created by gduval on 15/04/2022.
//

#ifndef GTKMM_MY_CW_CURLFORM_HPP
#define GTKMM_MY_CW_CURLFORM_HPP

#include <gtkmm/hvbox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include <curl/curl.h>
#include <iostream>

class My_CW_CurlForm : public Gtk::Window {

protected:

    enum _eMethod{
        GET,
        POST,
    };
    CURL * _pCurl;
    std::string _url =      "https://example.com";  // Replace with your desired URL;
    std::string _data =     "test";  // Replace with your data;

    int _w = 400; // width
    int _h = 300; // height

    Gtk::Entry _entry;
    Gtk::Button _button;
    Gtk::VBox _layout;

    void on_button_clicked() {
        _data = _entry.get_text();
    }

    bool init_curl();


public:
    My_CW_CurlForm();

    ~My_CW_CurlForm() override;
};

#endif // GTKMM_MY_CW_CURLFORM_HPP
