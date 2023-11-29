//
// Created by gduval on 15/04/2022.
//
#include <iostream>
#include "include/gtkmm/My_CW_Form1.hpp"

using namespace std;

My_CW_Form1::My_CW_Form1() {
    cout << __FUNCTION__ << " (instanciate)" << endl;

    set_title(__FUNCTION__);
    set_default_size(400, 300);

    // Create a VBox to hold the entry, button, and text view
    Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());

    _entry.set_placeholder_text("Enter data to POST");
    _button.set_label("POST");
    _button.signal_clicked().connect(sigc::mem_fun(*this, &My_CW_Form1::on_button_clicked));

    vbox->pack_start(_entry, Gtk::PACK_SHRINK);
    vbox->pack_start(_button, Gtk::PACK_SHRINK);

    // Add the VBox to the window
    add(*vbox);

    auto res = curl_init();
    cout << __FUNCTION__ << "curl_init : " << (res ? "OK":"KO") << endl;

    // Create a TextView widget
    _text_view.set_editable(false);
    _text_view.set_cursor_visible(false);

    // Create a ScrolledWindow to contain the TextView
    _scrolled_window.add(_text_view);

    // Set the HTML response content in the TextView
    const Glib::ustring html_response = R"(
        <!doctype html>
        <html>
        <!-- Your HTML content here -->
        </html>
    )";
    _text_buffer = _text_view.get_buffer();
    _text_buffer->set_text(html_response);

    // Add the ScrolledWindow to the VBox
    vbox->pack_start(_scrolled_window, Gtk::PACK_EXPAND_WIDGET);

    show_all();
}


My_CW_Form1::~My_CW_Form1() {
    cout << __FUNCTION__ << " (delete)" << endl;

}

bool My_CW_Form1::curl_init() {
    cout << __FUNCTION__ << endl;
    _pCurl = curl_easy_init();
    if (_pCurl) {
        return true;
    }else{
        cerr << __FUNCTION__ <<" failure !" << endl;
        return false;
    }
}

size_t curl_write_CB(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    auto* buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<const char*>(contents), total_size);
    cout << __FUNCTION__ <<" total_size=" << total_size << endl;
    return total_size;
}

bool My_CW_Form1::curl_send() {
    cout << __FUNCTION__ << endl;
    if (!curl_init() ) return false;

    curl_easy_setopt(_pCurl, CURLOPT_URL, _url.c_str());
    curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS, _data.c_str());

    // Create a buffer to store the response
    std::string response_buffer;
    curl_easy_setopt(_pCurl, CURLOPT_WRITEFUNCTION, curl_write_CB);
    curl_easy_setopt(_pCurl, CURLOPT_WRITEDATA, &response_buffer);

    CURLcode res = curl_easy_perform(_pCurl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform error: " << curl_easy_strerror(res) << std::endl;
        return false;
    } else {
        std::cout << "curl_easy_perform OK: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(_pCurl);

    // Update the text buffer with the response content
    Glib::RefPtr<Gtk::TextBuffer> text_buffer = _text_view.get_buffer();
    text_buffer->set_text(response_buffer);

    return true;
}

void My_CW_Form1::on_button_clicked()    {
    _data = _entry.get_text();
    curl_send();
}





