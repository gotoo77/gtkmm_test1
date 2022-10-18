cmake_minimum_required(VERSION 3.20)
project(gtkmm_test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

#------------------------ Program version --------------------------------#
set(PROGRAM_VERSION ${PROJECT_VERSION})
set(PROGRAM_NAME ${PROJECT_NAME})
configure_file(Version.h.in ${PROJECT_SOURCE_DIR}/external/Version.h)

if (WIN32)
set(EXTRA_WIN_PORTS_FILES
        src/win_ports/syslog.c
#        include/win_ports/syslog.h
        )
endif()
add_executable(${PROJECT_NAME}
        #        base.cpp
        src/main.cpp
        src/myCustomWindow.cpp
        include/myCustomWindow.hpp
        include/utils/AutoClose.hpp
        src/utils/read_val.cpp
        src/utils/MyLog.cpp
        ${EXTRA_WIN_PORTS_FILES}
        )

if (WIN32)
else()
    set(gtk_dir         /usr/include/gtk-3.0)
    set(glib_dir        /usr/include/glib-2.0)
    set(glib2_dir       /usr/lib64/glib-2.0/include/)
    set(pango_dir       /usr/include/pango-1.0)
    set(cairo_dir       /usr/include/cairo)
    set(gdkpixbuf_dir   /usr/include/gdk-pixbuf-2.0)
    set(atk_dir         /usr/include/atk-1.0/)
    set(gtkmm_dir       /usr/include/gtkmm-3.0)
    set(glibmm_dir      /usr/include/glibmm-2.4)
    set(sigc_dir        /usr/include/sigc++-2.0)
endif()

if (WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE
        C:/msys64/mingw64/include/gtkmm-3.0
        C:/msys64/mingw64/include/glibmm-2.4
        C:/msys64/mingw64/include/giomm-2.4
        C:/msys64/mingw64/lib/gtkmm-3.0/include
        C:/msys64/mingw64/lib/glibmm-2.4/include
        C:/msys64/mingw64/lib/gdkmm-3.0/include
        C:/msys64/mingw64/lib/pangomm-1.4/include
        C:/msys64/mingw64/include/glib-2.0
        C:/msys64/mingw64/lib/glib-2.0/include      #glibconfig.h
        C:/msys64/mingw64/include/sigc++-2.0        #sigc++/sigc++.h
        C:/msys64/mingw64/lib/sigc++-2.0/include    #sigc++config.h
        C:/msys64/mingw64/include/gtk-3.0           #gtk/gtk.h
        C:/msys64/mingw64/lib/giomm-2.4/include     #giommconfig.h
        C:/msys64/mingw64/include/pango-1.0         #pango/pango.h
        C:/msys64/mingw64/include/harfbuzz          #hb.h
        C:/msys64/mingw64/include/cairo             #cairo.h
        C:/msys64/mingw64/include/gdk-pixbuf-2.0    #gdk-pixbuf/gdk-pixbuf.h
        C:/msys64/mingw64/include/atk-1.0           #atk/atk.h
        C:/msys64/mingw64/include/gdkmm-3.0         #gdkmm/dragcontext.h
        C:/msys64/mingw64/include/cairomm-1.0       #cairomm/surface.h
        C:/msys64/mingw64/include/freetype2         #ft2build.h
        C:/msys64/mingw64/include                   #fontconfig/fontconfig.h
        C:/msys64/mingw64/lib/cairomm-1.0/include   #cairommconfig.h
        C:/msys64/mingw64/include/pangomm-1.4       #pangomm/context.h
        C:/msys64/mingw64/include/atkmm-1.6/        #atkmm/object.h
        C:/msys64/mingw64/lib/atkmm-1.6/include     #atkmmconfig.h
    )
else()
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
endif()

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        )

if (WIN32)
    target_link_libraries(${PROJECT_NAME}
            C:/msys64/mingw64/bin/libgtkmm-3.0-1.dll
            C:/msys64/mingw64/bin/libgtk-3-0.dll
            C:/msys64/mingw64/bin/libglib-2.0-0.dll
            C:/msys64/mingw64/bin/libgio-2.0-0.dll
            C:/msys64/mingw64/bin/libglibmm-2.4-1.dll
            C:/msys64/mingw64/bin/libatkmm-1.6-1.dll
            C:/msys64/mingw64/bin/libsigc-2.0-0.dll
            # add ws2_32 to solve "undefined reference to `__imp_WSACleanup' etc.."
            ws2_32
        )
else()
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
endif()
