SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
TARGET := x86
CFLAGS := -g

$(TARGET): $(SRC:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ) $(TARGET)

re: clean $(TARGET)