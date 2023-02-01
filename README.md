# LINUX notes :
* on Linux, add NO_AT_BRIDGE=1 to environment variables to prevent error like this
```
(gtkmm_test1:3311): dbind-WARNING **: 17:42:28.528: Couldn't connect to accessibility bus: Failed to connect to socket /tmp/dbus-K40ZRvHjhC: Connexion refused
```
run with a command/script like this:
```shell
export NO_AT_BRIDGE=1;
./gtkmm_test1
```
or in **CLion** IDE, add following to your binary target `gtkmm_test1`
```
NO_AT_BRIDGE=1
```

# WINDOWS notes :
## syslog for windows
tools :
* eventlog-to-syslog : 

`eventlog-to-syslog` is a tool or software application that allows you to convert 
Windows Event Logs into syslog messages and send them to a syslog server. 
The purpose of this tool is to allow Windows administrators to collect and
centralize logs from multiple Windows machines into a single location for easier
analysis and reporting.

> see : https://code.google.com/archive/p/eventlog-to-syslog/downloads 
> (Evtsys_4.5.1_64-Bit-LP.zip for instance)
* visual-syslog-server :
  `visual-syslog-server` : Visual Syslog Server is a software application that acts
as a syslog server, collecting and displaying syslog messages sent from network
devices in real-time. It provides a graphical user interface for monitoring and
analyzing syslog messages, making it easier to manage and understand large amounts
of log data. The software may offer features such as message filtering, search, 
and alerts, among others. The purpose of this tool is to help administrators and 
IT professionals manage and monitor their network devices more efficiently.
> see : https://maxbelkov.github.io/visualsyslog/ (visualsyslog_setup.zip for instance)

# installing Gtkmm using :
* WINDOWS **[MSYS](https://www.msys2.org/)** (Software Distribution and Building Platform for Windows) 
* `pacman` [view](https://wiki.gnome.org/Projects/gtkmm/MSWindows)

## pkg-config
### gtkmm
```shell
pkg-config gtkmm-3.0 --libs
```
example :
```
gotoo77@DESKTOP-BO70FKC MINGW64 ~
$ pkg-config gtkmm-3.0 --libs
-L/mingw64/lib -lgtkmm-3.0 -latkmm-1.6 -lgdkmm-3.0 -lgiomm-2.4 -lgtk-3 -lgdk-3 -lz -lgdi32 -limm32 -lshell32 -lole32 -Wl,-luuid -lwinmm -ldwmapi -lsetupapi -lcfgmgr32 -lhid -lwinspool -lcomctl32 -lcomdlg32 -lpangowin32-1.0 -latk-1.0 -lcairo-gobject -lgio-2.0 -lpangomm-1.4 -lglibmm-2.4 -lcairomm-1.0 -lsigc-2.0 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -lcairo -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0 -lintl
```
### glibmm
```shell
pkg-config glibmm-2.4 --libs
```
example :
```
gotoo77@DESKTOP-BO70FKC MINGW64 ~
$ pkg-config glibmm-2.4 --libs
-L/mingw64/lib -lglibmm-2.4 -lgobject-2.0 -lglib-2.0 -lintl -lsigc-2.0
```

## Issues encountered
### WINDOWS 
* crash at runtime for now
```Process finished with exit code -1073741515 (0xC0000135)```. 
 Apparently, ``0xc0000135`` is the STATUS_DLL_NOT_FOUND
