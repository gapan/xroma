CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lX11
PREFIX = /usr/local/

SRC = xroma.c
OBJ = $(SRC:.c=.o)
TARGET = xroma
MANPAGE = xroma.1

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)/$(PREFIX)/bin/$(TARGET)
	install -Dm644 $(MANPAGE) $(DESTDIR)/usr/share/man/man1/$(MANPAGE)

uninstall:
	rm -f $(DESTDIR)/$(PREFIX)/$(TARGET)
	rm -f $(DESTDIR)/usr/share/man/man1/$(MANPAGE)
