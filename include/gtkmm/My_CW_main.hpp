//
// Created by gduval on 15/04/2022.
//

#ifndef GTKMM_MY_CW_MAIN_HPP
#define GTKMM_MY_CW_MAIN_HPP

#include <gtkmm/hvbox.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class My_CW_main : public Gtk::Window {
private:
    /**
     * test si le fichier existe
     * @param name
     * @return
     */
    static inline bool check_file_exist(const std::string &name) {
        struct stat buffer{};
        return (stat(name.c_str(), &buffer) == 0);
    }

protected:
    Gtk::VBox                   _vbox_one;          // un vertical box dans laquelle entrer les elements (widgets)
    Gtk::Label                  _label_one;         // un label (zone de texte)
    Gtk::Entry                  _entry_one;         // une saisie (edit)
    Gtk::Button                 _button_one;        // un bouton
    Gtk::ArrowType              _arrow;             //
    std::vector<Gtk::Button>    _buttons;           // une liste de boutons

    void on_btn_clicked_My_CW_Form1();

    void on_btn_clicked_My_CW_pouet();
    /**
     * fonction evenement avec parametres
     * @param id
     * @param data
     */
    void on_btn_clic(int id, const Glib::ustring& data);

public:
    My_CW_main();
    /**
     * constructeur avec une liste de bouton et un fichier image
     * @param f_btn_list liste de bouton a creer
     * @param f_img_file fichier image a utiliser pour la fenetre programme
     */
    explicit My_CW_main(const std::vector<std::string>& f_btn_list, const std::string &f_img_file);

    ~My_CW_main() override;
};

#endif // GTKMM_MY_CW_MAIN_HPP
