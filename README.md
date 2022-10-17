note : add NO_AT_BRIDGE=1 to environment variables to prevent error like this

```
(gtkmm_test1:3311): dbind-WARNING **: 17:42:28.528: Couldn't connect to accessibility bus: Failed to connect to socket /tmp/dbus-K40ZRvHjhC: Connexion refused
```

```shell
export NO_AT_BRIDGE=1;
./gtkmm_test1
```

or in CLion add following to your binary target `gtkmm_test1`

```
NO_AT_BRIDGE=1
```

