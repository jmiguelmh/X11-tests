#include <X11/Xlib.h>
#include <iostream>

#define ESC_KEY 9

int main(void)
{
    Display *d;
    Window w;
    XEvent e;
    int s;
    bool exit = false;

    d = XOpenDisplay(NULL);
    if (d == NULL)
    {
        std::cout << "Cannot open display\n";
        return -1;
    }

    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 100, 100, 640, 480, 1, BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);


    while (!exit)
    {
        XNextEvent(d, &e);
        if (e.type == KeyPress)
        {
            switch (e.xkey.keycode)
            {
            case ESC_KEY:
                exit = true;
                break;
            }
        }
    }

    XCloseDisplay(d);
    return 0;
}