//
// Created by gotoo77 on 17/10/2022.
//
#include <gtkmm.h>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "test gtkmm - from " << __FILE__ << std::endl;
    auto app =
            Gtk::Application::create(argc, argv,
                                     "org.gtkmm.examples.base");

    Gtk::Window window;
    window.set_default_size(200, 200);

    return app->run(window);
}
