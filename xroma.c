#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

void rgb_to_hsv(int r, int g, int b, double *h, double *s, double *v) {
    double max = (r > g) ? (r > b ? r : b) : (g > b ? g : b);
    double min = (r < g) ? (r < b ? r : b) : (g < b ? g : b);
    double delta = max - min;

    if (delta == 0) {
        *h = 0;  // undefined, but commonly set to 0
    } else if (max == r) {
        *h = 60 * ((g - b) / delta) + 0;
    } else if (max == g) {
        *h = 60 * ((b - r) / delta) + 120;
    } else {
        *h = 60 * ((r - g) / delta) + 240;
    }

    // Ensure hue is in the range [0, 360)
    if (*h < 0) {
        *h += 360;
    }
    *s = (max == 0) ? 0 : 100 * delta / max;
    *v = 100 * max / 255.0;
}

void print_color(int r, int g, int b, int hex, int hsv) {
    if (hex) {
        printf("#%02X%02X%02X\n", r, g, b);
    } else if (hsv) {
        double h, s, v;
        rgb_to_hsv(r, g, b, &h, &s, &v);
        printf("%.2f %.2f %.2f\n", h, s, v);
    } else {
        printf("%d %d %d\n", r, g, b);
    }
}

void print_help() {
    printf("Usage: xroma [options]\n");
    printf("Options:\n");
    printf("  -h, --help            Print this help message\n");
    printf("      --hex             Print color in hexadecimal format\n");
    printf("      --hsv             Print color in HSV format\n");
    printf("  -c, --coordinates X,Y Specify the coordinates (X,Y) to read the color from\n");
}

int main(int argc, char *argv[]) {
    Display *display;
    Window root;
    XEvent ev;

    int hex = 0;
	int hsv = 0;
    int custom_coordinates = 0;
    int x = 0, y = 0;

    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"hex", no_argument, 0, 0},
        {"hsv", no_argument, 0, 1},
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
			case 1:
				hsv = 1;
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

    print_color(color.red / 256, color.green / 256, color.blue / 256, hex, hsv);

    XDestroyImage(image);
    XCloseDisplay(display);

    return 0;
}
