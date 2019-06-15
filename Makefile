CC = clang
CFLAGS = -std=c99 -Ilib -Wall -Wextra -Wshadow -Wswitch-enum -Wno-unused-parameter -Wno-missing-braces
LDFLAGS = -ldl -lglfw -lGL -lm

SRC_FILES = game.c glad.c platform_linux.c shader.c
SRC = $(addprefix src/, $(SRC_FILES))
EXE_FILE = quaternion_demo

LIB_FILES = game.c glad.c shader.c
LIB = $(addprefix src/, $(LIB_FILES))
LIB_NAME = game.so

DBGDIR = build/debug
DBGEXE = $(DBGDIR)/$(EXE_FILE)
DBGCFLAGS = -g -Og -Werror

RELDIR = build/release
RELEXE = $(RELDIR)/$(EXE_FILE)
RELLIB = $(RELDIR)/$(LIB_NAME)
RELLIBTMP = $(RELLIB).tmp
RELCFLAGS = -DPLATFORM_HOTLOAD_GAME_CODE -O2 -Os

EMS_FILES = game.c platform_emscripten.c shader.c
EMSSRC = $(addprefix src/, $(EMS_FILES))
EMSDIR = build/emscripten
EMSEXE = $(EMSDIR)/$(EXE_FILE).html
EMSCFLAGS = --preload-file shader -s USE_GLFW=3 -s USE_WEBGL2=1

.PHONY: default all build_debug build_lib build_release clean debug dir_debug dir_release emscripten memcheck run todo

default: build_release

all: build_debug build_release emscripten

build_debug: dir_debug
	$(CC) $(CFLAGS) $(DBGCFLAGS) $(SRC) -o $(DBGEXE) $(LDFLAGS)

build_lib:
	$(CC) $(CFLAGS) -fpic -shared $(RELCFLAGS) $(LIB) -o $(RELLIBTMP) $(LDFLAGS)
	mv $(RELLIBTMP) $(RELLIB)

build_release: dir_release build_lib
	$(CC) $(CFLAGS) $(RELCFLAGS) $(SRC) -o $(RELEXE) $(LDFLAGS)

clean:
	rm -f $(RELDIR)/* $(DBGDIR)/*

debug: build_debug
	cgdb $(DBGEXE)

dir_debug:
	@mkdir -p $(DBGDIR)

dir_release:
	@mkdir -p $(RELDIR)

memcheck: build_debug
	valgrind --track-origins=yes ./$(DBGEXE)

run: build_release
	./$(RELEXE)

emscripten:
	@mkdir -p $(EMSDIR)
	emcc $(CFLAGS) $(EMSCFLAGS) -O2 -Os $(EMSSRC) -o $(EMSEXE) $(LDFLAGS)

todo:
	@grep -FIR --colour=never --ignore-case --line-number todo src/ \
	| sed -re  's/^([^:]+):[[:space:]]*(.*)/\1\x01\2/' \
	| sed -re  's/^([^:]+):[[:space:]]*(.*)/\1\x01\2/' \
	| column -s $$'\x01' -t
