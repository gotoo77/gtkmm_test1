#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include "myCustomWindow.hpp"

int demo1(int argc, char **argv) {
    std::cout << "demo1" << std::endl;

    Gtk::Main app(argc, argv);
    Gtk::Window w;
//    Gtk::VBox vb;
    Gtk::Box my_box1;
//    w.add(vb);
    w.add(my_box1);

    Gtk::Label l;
    l.set_label("mon_label1");
    my_box1.pack_start(l);
    l.show();

    Gtk::Entry e;
    my_box1.pack_start(e);
    e.show();

    Gtk::Button my_btn1("my_btn1 button!");
    my_box1.pack_start(my_btn1);
    my_btn1.show();
    my_box1.show();

    Gtk::Main::run(w);
    return 0;
}

int demo_CustomWindow(int argc, char **argv) {
    Gtk::Main app(argc, argv);
//    myCustomWindow w;
    std::string img = "/media/sf_VM_linux/tests/gtkmm_test1/assets/img/logocpp64x64.png";
    std::vector<std::string> btn_list = {"1", "trotro", "2", "yolo", "pouet", "gros bouton badass", "3",
                                         "tata monique"};

    myCustomWindow w(btn_list, img);
    Gtk::Main::run(w);
    return 0;
}


int main(int argc, char **argv) {
    std::cout << "Hello, gtkmm_test1 " << std::endl;
    if (argc>=2){
        if (std::string(argv[0])=="demo1"){
            return demo1(argc, argv);
        }
        else{
            return demo_CustomWindow(argc, argv);
        }
    }
    return 0;

}