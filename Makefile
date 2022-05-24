SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
TARGET := ./x86
CFLAGS := -g -Werror
INPUT_SRC := program.S
INPUT := $(INPUT_SRC:.S=.bin)

$(TARGET): $(SRC:.c=.o)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ) $(TARGET) $(INPUT)

re: clean $(TARGET)

run: $(TARGET) $(INPUT)
	$(TARGET) $(INPUT)

$(INPUT): $(INPUT_SRC)
	nasm -o $@ $^
	# objcopy -O binary $@
	
.PHONY: disas

disas: $(INPUT)
	objdump -D -b binary -m i386:x86-64 $^