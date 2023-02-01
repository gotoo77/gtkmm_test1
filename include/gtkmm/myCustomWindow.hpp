//
// Created by gduval on 15/04/2022.
//

#ifndef GTKMM_TEST1_MYCUSTOMWINDOW_HPP
#define GTKMM_TEST1_MYCUSTOMWINDOW_HPP

#include <gtkmm/hvbox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class myCustomWindow : public Gtk::Window {
private:
    /**
     * test si le fichier exitse
     * @param name
     * @return
     */
    static inline bool check_file_exist(const std::string &name) {
        struct stat buffer{};
        return (stat(name.c_str(), &buffer) == 0);
    }

protected:
    Gtk::VBox                   _vbox_one;            // un vertical box dans laquelle entrer les elements (widgets)
    Gtk::Label                  _label_one;          // un label (zone de texte)
    Gtk::Entry                  _entry_one;          // une saisie (edit)
    Gtk::Button                 _button_one;        // un bouton
    Gtk::ArrowType                 _arrow;        //
    std::vector<Gtk::Button>    _buttons;           // une liste de boutons

    void on_btn_clicked1();

    void on_btn_clicked2();
    /**
     * fonction evenement avec parametres
     * @param id
     * @param data
     */
    void on_btn_clic(int id, const Glib::ustring& data);

public:
    myCustomWindow();
    /**
     * constructeur avec une liste de bouton et un fichier image
     * @param f_btn_list liste de bouton a creer
     * @param f_img_file fichier image a utiliser pour la fenetre programme
     */
    explicit myCustomWindow(const std::vector<std::string>& f_btn_list, const std::string &f_img_file);

    ~myCustomWindow() override;
};

#endif //GTKMM_TEST1_MYCUSTOMWINDOW_HPP
