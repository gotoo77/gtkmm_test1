cmake_minimum_required(VERSION 3.20)
project(gtkmm_test1)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
        main.cpp
        myCustomWindow.cpp
        myCustomWindow.h)

SET(gtk_dir /usr/include/gtk-3.0)
SET(glib_dir /usr/include/glib-2.0)
SET(glib2_dir /usr/lib64/glib-2.0/include/)
SET(pango_dir /usr/include/pango-1.0)
SET(cairo_dir /usr/include/cairo)
SET(gdkpixbuf_dir /usr/include/gdk-pixbuf-2.0)
SET(atk_dir /usr/include/atk-1.0/)
SET(gtkmm_dir /usr/include/gtkmm-3.0)
SET(glibmm_dir /usr/include/glibmm-2.4)
SET(sigc_dir /usr/include/sigc++-2.0)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${gtk_dir}"
        "${glib_dir}"
        "${glib2_dir}"
        "${pango_dir}"
        "${cairo_dir}"
        "${gdkpixbuf_dir}"
        "${atk_dir}"
        "${gtkmm_dir}"
        "${glibmm_dir}"
        "${sigc_dir}"
        /usr/lib64/glibmm-2.4/include
        /usr/lib64/sigc++-2.0/include
        /usr/include/gdkmm-3.0/
        /usr/lib64/gdkmm-3.0/include
        /usr/lib64/pangomm-1.4/include
        /usr/lib64/gtkmm-3.0/include
        /usr/include/pangomm-1.4
        /usr/include/cairomm-1.0
        /usr/include/freetype2
        /usr/include/atkmm-1.6
        /usr/include/giomm-2.4
        /usr/lib64/giomm-2.4/include
        )

target_link_libraries(${PROJECT_NAME}
        /usr/lib64/libgtkmm-3.0.so
        /usr/lib64/libgtk-3.so
        /usr/lib64/libgobject-2.0.so
        /usr/lib64/libglib-2.0.so
        /usr/lib64/libgio-2.0.so
        /usr/lib64/libglibmm-2.4.so
        /usr/lib64/libatkmm-1.6.so
        /usr/lib64/libsigc-2.0.so
        )