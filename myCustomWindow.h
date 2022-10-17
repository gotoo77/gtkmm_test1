//
// Created by gduval on 15/04/2022.
//

#ifndef GTKMM_TEST1_MYCUSTOMWINDOW_H
#define GTKMM_TEST1_MYCUSTOMWINDOW_H

#include <gtkmm/hvbox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class myCustomWindow : public Gtk::Window {
private:
    static inline bool check_file_exist(const std::string &name);

protected:
    Gtk::VBox vbox_one_;
    Gtk::Label label_one_;
    Gtk::Entry entry_one_;
    Gtk::Button button_one_;
    std::vector<Gtk::Button> buttons_;

    void on_btn_clicked1();

    void on_btn_clicked2();

    void on_btn_clic(int id, Glib::ustring data);

public:
    myCustomWindow();

    explicit myCustomWindow(std::vector<std::string> f_btn_list, const std::string &f_img_file);

    ~myCustomWindow() override;
};

#endif //GTKMM_TEST1_MYCUSTOMWINDOW_H
