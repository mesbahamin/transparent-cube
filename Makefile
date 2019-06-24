CC = clang
CFLAGS = -std=c11 -Ilib -Wall -Wextra -Wshadow -Wswitch-enum -Wno-unused-parameter -Wno-missing-braces
LDFLAGS = -ldl -lglfw -lGL

SRC_FILES = platform_linux.c
SRC = $(addprefix src/, $(SRC_FILES))
EXE_FILE = transparent_cube

LIB_FILES = game.c
LIB = $(addprefix src/, $(LIB_FILES))
LIB_NAME = game.so

DBGDIR = out/debug
DBGEXE = $(DBGDIR)/$(EXE_FILE)
DBGCFLAGS = -g -Og -Werror

RELDIR = out/release
RELEXE = $(RELDIR)/$(EXE_FILE)
RELLIB = $(RELDIR)/$(LIB_NAME)
RELLIBTMP = $(RELLIB).tmp
RELCFLAGS = -DPLATFORM_HOTLOAD_GAME_CODE -O2 -Os

.PHONY: default all build_debug build_lib build_release clean debug dir_debug dir_release memcheck run todo wasm

default: build_release

all: build_debug build_release wasm

build_debug: dir_debug
	$(CC) $(CFLAGS) $(DBGCFLAGS) $(SRC) -o $(DBGEXE) $(LDFLAGS)

build_lib:
	$(CC) $(CFLAGS) -fpic -shared $(RELCFLAGS) $(LIB) -o $(RELLIBTMP) $(LDFLAGS)
	mv $(RELLIBTMP) $(RELLIB)

build_release: dir_release build_lib
	$(CC) $(CFLAGS) $(RELCFLAGS) $(SRC) -o $(RELEXE) $(LDFLAGS)

clean:
	rm -rf out/

debug: build_debug
	gdb $(DBGEXE)

dir_debug:
	@mkdir -p $(DBGDIR)

dir_release:
	@mkdir -p $(RELDIR)

dir_wasm:
	@mkdir -p $(EMSDIR)

memcheck: build_debug
	valgrind --track-origins=yes ./$(DBGEXE)

run: build_release
	./$(RELEXE)

todo:
	@grep -FIR --colour=never --ignore-case --line-number todo src/ \
	| sed -re  's/^([^:]+):[[:space:]]*(.*)/\1\x01\2/' \
	| sed -re  's/^([^:]+):[[:space:]]*(.*)/\1\x01\2/' \
	| column -s $$'\x01' -t

wasm:
	./build_wasm.sh
