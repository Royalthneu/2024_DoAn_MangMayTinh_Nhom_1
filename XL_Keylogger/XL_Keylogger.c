#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

void logKeys() {
    Display *dpy = XOpenDisplay(0);
    char keys[32];
    while(1) {
        XQueryKeymap(dpy, keys);
        for (int i = 0; i < 32; i++) {
            for (int j = 0; j < 8; j++) {
                if (keys[i] & (1 << j)) {
                    printf("Key %d pressed\n", i * 8 + j);
                }
            }
        }
        usleep(50000);  // Sleep for 50ms
    }
    XCloseDisplay(dpy);
}

int main() {
    logKeys();
    return 0;
}
