//#include <gtk/gtk.h>
//#include <gtk/gtklabel.h>
//#include <glib-2.0/gio/gdbusconnection.h>
//#include <gtk/gtkwidget.h>
#include <iostream>

//static gboolean name_seen;
//static GtkWidget *placeholder;
//
//static void
//on_name_appeared(GDBusConnection *connection,
//                 const gchar *name,
//                 const gchar *name_owner,
//                 gpointer user_data) {
//    name_seen = TRUE;
//}
//
//static void
//on_name_vanished(GDBusConnection *connection,
//                 const gchar *name,
//                 gpointer user_data) {
//    if (!name_seen)
//        return;
//
//    if (placeholder) {
//        gtk_widget_destroy(placeholder);
//        g_object_unref(placeholder);
//        placeholder = nullptr;
//    }
//}
//
//#ifdef G_OS_WIN32
//#define APP_EXTENSION ".exe"
//#else
//#define APP_EXTENSION
//#endif
//
//GtkWidget *
//do_application_demo(GtkWidget *toplevel) {
//    static guint watch = 0;
//
//    if (watch == 0)
//        watch = g_bus_watch_name(G_BUS_TYPE_SESSION,
//                                 "org.gtk.Demo2",
//                                 static_cast<GBusNameWatcherFlags>(0),
//                                 on_name_appeared,
//                                 on_name_vanished,
//                                 nullptr, nullptr);
//
//    if (placeholder == nullptr) {
//        const gchar *command;
//        GError *error = nullptr;
//
//        if (g_file_test("./gtk3-demo-application" APP_EXTENSION, G_FILE_TEST_IS_EXECUTABLE))
//            command = "./gtk3-demo-application" APP_EXTENSION;
//        else
//            command = "gtk3-demo-application";
//
//        if (!g_spawn_command_line_async(command, &error)) {
//            g_warning ("%s", error->message);
//            g_error_free(error);
//        }
//
//        placeholder = gtk_label_new("");
//        g_object_ref_sink(placeholder);
//    } else {
//        g_dbus_connection_call_sync(g_bus_get_sync(G_BUS_TYPE_SESSION, nullptr, nullptr),
//                                    "org.gtk.Demo2",
//                                    "/org/gtk/Demo2",
//                                    "org.gtk.Actions",
//                                    "Activate",
//                                    g_variant_new("(sava{sv})", "quit", nullptr, nullptr),
//                                    nullptr,
//                                    static_cast<GDBusCallFlags>(0),
//                                    G_MAXINT,
//                                    nullptr, nullptr);
//    }
//
//    return placeholder;
//}

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/hvbox.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include "myCustomWindow.h"

int main_demo1(int argc, char **argv) {
    std::cout << "main_demo1" << std::endl;

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

int main_CustomWindow(int argc, char **argv) {
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
//    return main_demo1(argc,argv);
    return main_CustomWindow(argc, argv);
}