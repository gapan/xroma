CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lX11

SRC = xroma.c
OBJ = $(SRC:.c=.o)
TARGET = xroma

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
