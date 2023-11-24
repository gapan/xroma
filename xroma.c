#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

void print_color(int r, int g, int b, int hex) {
    if (hex) {
        printf("#%02X%02X%02X\n", r / 256, g / 256, b / 256);
    } else {
        printf("%d %d %d\n", r / 256, g / 256, b / 256);
    }
}

void print_help() {
    printf("Usage: xroma [options]\n");
    printf("Options:\n");
    printf("  -h, --help            Print this help message\n");
    printf("      --hex             Print color in hexadecimal format\n");
    printf("  -c, --coordinates X,Y Specify the coordinates (X,Y) to read the color from\n");
}

int main(int argc, char *argv[]) {
    Display *display;
    Window root;
    XEvent ev;

    int hex = 0;
    int custom_coordinates = 0;
    int x = 0, y = 0;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"hex", no_argument, 0, 0},
        {"coordinates", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    int option;
    int option_index = 0;

    while ((option = getopt_long(argc, argv, "hc:", long_options, &option_index)) != -1) {
        switch (option) {
            case 'h':
                print_help();
                return 0;
            case 0:
                hex = 1;
                break;
            case 'c':
                custom_coordinates = 1;
                sscanf(optarg, "%d,%d", &x, &y);
                break;
            case '?':
                return 1;
            default:
                abort();
        }
    }

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open display\n");
        return 1;
    }

    root = DefaultRootWindow(display);

    if (!custom_coordinates) {
        XQueryPointer(display, root, &ev.xbutton.root, &ev.xbutton.window,
                      &ev.xbutton.x_root, &ev.xbutton.y_root,
                      &ev.xbutton.x, &ev.xbutton.y, &ev.xbutton.state);
        x = ev.xbutton.x;
        y = ev.xbutton.y;
    }

    XColor color;
    XImage *image = XGetImage(display, root, x, y, 1, 1, AllPlanes, ZPixmap);

    color.pixel = XGetPixel(image, 0, 0);
    XQueryColor(display, DefaultColormap(display, DefaultScreen(display)), &color);

    print_color(color.red, color.green, color.blue, hex);

    XDestroyImage(image);
    XCloseDisplay(display);

    return 0;
}
