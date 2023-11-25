# xroma

`xroma` is a simple command-line tool for retrieving and displaying the
color information of a pixel on the screen. It utilizes the Xlib library
to interact with the X Window System.

By default it reads the color information for the pixel that is
under the mouse cursor, but you can also specify the coordinates of an
arbitrary pixel. The output is provided by default in a set of RGB
values (range 0-255), but you can also get it in hexadecimal format.

The name xroma comes from the greek word chroma, which means color. The
"ch" part is actually written with an X in greek, which is not pronounced
like an english X, but rather somewhat like the "ch" sound in chroma.

## Example Usage

Print the color information of the pixel under the mouse pointer:

```bash
$ xroma
53 132 228
```

Print the color information at specific coordinates:

```bash
$ xroma -c 100,200
198 75 75
```

Print the color information in hexadecimal format:

```bash
$ xroma --hex
#D96B5F
```

## Dependencies

* Xlib

