gcc -o build/desktopplayer src/main.c $(pkg-config --cflags --libs gtk+-3.0 gtk-layer-shell-0)
