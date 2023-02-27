# VAR
# -----
CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
#CFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter
CFLAGS += -Wno-unused-parameter
CFLAGS += -I lib/glad/include -I lib/stb -I lib/cglm/include
LDFLAGS += -lglfw -lm
BIN = bin
SRC = src
EXE = breakout
GDB_SCRIPT = gdb-script

.PHONY: all
all: $(BIN) $(BIN)/$(EXE)


# Build libraries
# ---------------------
LIB_OBJ = lib/glad/src/glad.o lib/cglm/libcglm.a

lib/glad/src/glad.o:
	cd lib/glad && $(CC) -o src/glad.o -I include -c src/glad.c

lib/cglm/libcglm.a:
	cd lib/cglm && cmake . -DCGLM_STATIC=ON && make

# Define all objects to build
# ---------------------------
OBJ = \
main.o \
game.o \
shader.o \
texture.o \
file_io.o \
resource_manager.o \
renderer.o \
game_object.o \
game_level.o \
ball_object.o \
collisions.o \
particle_manager.o \

# Object dependencies
# -------------------
MAIN_DEP = main.c \
game/game.h \
gfx/gfx.h \

GAME_DEP = game/game.c game/game.h \
gfx/gfx.h \
gfx/renderer.h \
gfx/resource_manager.h \
gfx/shader_type.h \
gfx/shader.h \
gfx/tex_type.h \
game/game_level.h \
game/game_object.h \
game/ball_object.h \
game/collisions.h \
game/particle_manager.h \

SHADER_DEP = gfx/shader.c gfx/shader.h \
gfx/gfx.h \

TEXTURE_DEP = gfx/texture.c gfx/texture.h \
gfx/gfx.h \

FILE_IO_DEP = util/file_io.c  util/file_io.h \

RESOURCE_MANAGER_DEP = gfx/resource_manager.c gfx/resource_manager.h \
gfx/gfx.h \
gfx/tex_type.h \
gfx/texture.h \
gfx/shader_type.h \
gfx/shader.h \
util/file_io.h \

RENDERER_DEP = gfx/renderer.c gfx/renderer.h \
gfx/gfx.h \
gfx/shader_type.h \
gfx/shader.h \
gfx/tex_type.h \
gfx/texture.h \
gfx/resource_manager.h \

GAME_OBJECT_DEP = game/game_object.c game/game_object.h \
gfx/tex_type.h \

GAME_LEVEL_DEP = game/game_level.c game/game_level.h \
gfx/renderer.h \
util/file_io.h \
game/game_object.h \

BALL_OBJECT_DEP = game/ball_object.c game/ball_object.h \
game/game_object.h \

COLLISIONS_DEP = game/collisions.c game/collisions.h \
game/ball_object.h \
game/game_object.h \

PARTICLE_MANAGER_DEP = game/particle_manager.c game/particle_manager.h \
game/game_object.h \

# Build objects
# -------------
$(BIN)/main.o: $(addprefix $(SRC)/,$(MAIN_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/game.o: $(addprefix $(SRC)/,$(GAME_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/shader.o: $(addprefix $(SRC)/,$(SHADER_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/texture.o: $(addprefix $(SRC)/,$(TEXTURE_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/file_io.o: $(addprefix $(SRC)/,$(FILE_IO_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/resource_manager.o: $(addprefix $(SRC)/,$(RESOURCE_MANAGER_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/renderer.o: $(addprefix $(SRC)/,$(RENDERER_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/game_object.o: $(addprefix $(SRC)/,$(GAME_OBJECT_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/game_level.o: $(addprefix $(SRC)/,$(GAME_LEVEL_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/ball_object.o: $(addprefix $(SRC)/,$(BALL_OBJECT_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/collisions.o: $(addprefix $(SRC)/,$(COLLISIONS_DEP))
	$(CC) -o $@ -c $< $(CFLAGS)

$(BIN)/particle_manager.o: $(addprefix $(SRC)/,$(PARTICLE_MANAGER_DEP))
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

.PHONY: debug
debug: all
	gdb -x $(GDB_SCRIPT) -tui

.PHONY: clean
clean:
	rm -rf $(BIN)
