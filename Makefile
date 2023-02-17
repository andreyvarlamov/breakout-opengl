CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter
CFLAGS += -I lib/glad/include
LDFLAGS = lib/glad/src/glad.o
LDFLAGS += -lglfw

SRC = $(wildcard src/**/*.c) $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all clean

all: dirs libs game

dirs:
	mkdir -p ./$(BIN)

libs:
	cd lib/glad && $(CC) -o src/glad.o -I include -c src/glad.c

# $@ evaluates to target name; $< evaluates to the first prerequisite
# $^ evaluates to all prerequisites
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)

run: all
	$(BIN)/game

clean:
	rm -rf $(BIN) $(OBJ)
