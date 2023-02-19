# VAR
# -----
CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter
CFLAGS += -I lib/glad/include
LDFLAGS += -lglfw
BIN = bin
SRC = src
EXE = breakout

.PHONY: all
all: $(BIN) $(BIN)/$(EXE)


# Build library objects
# ---------------------
LIB_OBJ = lib/glad/src/glad.o

lib/glad/src/glad.o:
	cd lib/glad && $(CC) -o src/glad.o -I include -c src/glad.c

# Build objects
# -------------
OBJ = main.o \
	  window.o

MAIN_DEP = main.c \
		   gfx/window.h \
		   gfx/gfx.h
$(BIN)/main.o: $(addprefix $(SRC)/,$(MAIN_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

WINDOW_DEP = gfx/window.c \
		     gfx/window.h \
			 gfx/gfx.h
$(BIN)/window.o: $(addprefix $(SRC)/,$(WINDOW_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)


# Main targets + Util
# -------------------
# $@ evaluates to target name; $< evaluates to the first prerequisite
# $^ evaluates to all prerequisites
$(BIN)/$(EXE): $(addprefix $(BIN)/,$(OBJ)) $(LIB_OBJ)
	$(CC) -o $(BIN)/$(EXE) $^ $(LDFLAGS)

$(BIN):
	mkdir -p $(BIN)

.PHONY: run
run: all
	$(BIN)/$(EXE)

.PHONY: clean
clean:
	rm -rf $(BIN) $(LIB_OBJ)
