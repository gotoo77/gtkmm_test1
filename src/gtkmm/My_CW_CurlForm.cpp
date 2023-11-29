//
// Created by gduval on 15/04/2022.
//
#include <iostream>
#include "include/gtkmm/My_CW_CurlForm.hpp"

using namespace std;

My_CW_CurlForm::My_CW_CurlForm() {
    cout << __FUNCTION__ << " (instanciate)" << endl;

    set_title(__FUNCTION__);
    set_default_size(_w, _h);

    _entry.set_placeholder_text("Enter data to POST");
    _button.set_label("POST");
    _button.signal_clicked().connect(sigc::mem_fun(*this, &My_CW_CurlForm::on_button_clicked));

    _layout.pack_start(_entry, Gtk::PACK_SHRINK);
    _layout.pack_start(_button, Gtk::PACK_SHRINK);

    add(_layout);

    auto res = init_curl();
    cout << __FUNCTION__ << "init_curl : " << (res ? "OK":"KO") << endl;

    show_all();
}


My_CW_CurlForm::~My_CW_CurlForm() {
    cout << __FUNCTION__ << " (delete)" << endl;
}

bool My_CW_CurlForm::init_curl() {
    cout << __FUNCTION__ << endl;
    _pCurl = curl_easy_init();
    if (_pCurl) {
        curl_easy_setopt(_pCurl, CURLOPT_URL, _url.c_str());
        curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS, _data.c_str());
        CURLcode res = curl_easy_perform(_pCurl);
        if (res != CURLE_OK) {
            std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
            return false;
        }
        curl_easy_cleanup(_pCurl);
    }else{
        return false;
    }
    return true;
}





