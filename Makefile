# VAR
# -----
CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter
CFLAGS += -I lib/glad/include -I lib/stb -I lib/cglm/include
LDFLAGS += -lglfw -lm
BIN = bin
SRC = src
EXE = breakout

.PHONY: all
all: $(BIN) $(BIN)/$(EXE)


# Build library objects
# ---------------------
LIB_OBJ = lib/glad/src/glad.o \
		  lib/cglm/libcglm.a

lib/glad/src/glad.o:
	cd lib/glad && $(CC) -o src/glad.o -I include -c src/glad.c

lib/cglm/libcglm.a:
	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make

# Build objects
# -------------
OBJ = main.o \
	  window.o \
	  game.o \
	  shader.o \
	  texture.o \
	  file_io.o \
	  resource_manager.o \
	  renderer.o

MAIN_DEP = main.c \
		   gfx/window.h \
		   gfx/gfx.h
$(BIN)/main.o: $(addprefix $(SRC)/,$(MAIN_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

WINDOW_DEP = gfx/window.c \
		     gfx/window.h \
			 gfx/gfx.h \
			 game.h
$(BIN)/window.o: $(addprefix $(SRC)/,$(WINDOW_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

GAME_DEP = game.c \
		   game.h
$(BIN)/game.o: $(addprefix $(SRC)/,$(GAME_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

SHADER_DEP = gfx/shader.c \
			 gfx/shader.h \
			 gfx/gfx.h
$(BIN)/shader.o: $(addprefix $(SRC)/,$(SHADER_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

TEXTURE_DEP = gfx/texture.c \
			  gfx/texture.h \
			  gfx/gfx.h
$(BIN)/texture.o: $(addprefix $(SRC)/,$(TEXTURE_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

FILE_IO_DEP = util/file_io.c \
			  util/file_io.h
$(BIN)/file_io.o: $(addprefix $(SRC)/,$(FILE_IO_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

RESOURCE_MANAGER_DEP = gfx/resource_manager.c \
					   gfx/resource_manager.h \
					   gfx/gfx.h \
					   gfx/tex_type.h \
					   gfx/texture.h \
					   gfx/shader_type.h \
					   gfx/shader.h
$(BIN)/resource_manager.o: $(addprefix $(SRC)/,$(RESOURCE_MANAGER_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

RENDERER_DEP = gfx/renderer.c \
			   gfx/renderer.h
$(BIN)/renderer.o: $(addprefix $(SRC)/,$(RENDERER_DEP))
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
	rm -rf $(BIN)
