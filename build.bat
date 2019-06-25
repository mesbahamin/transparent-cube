@echo off

set CFLAGS= ^
    -std=c99 ^
    -nostdlib ^
    -D_CRT_SECURE_NO_WARNINGS ^
    -Ilib ^
    -Ilib/glfw_windows/include ^
    -Wall ^
    -Wextra ^
    -Wshadow ^
    -Wswitch-enum ^
    -Wno-unused-parameter ^
    -Wno-missing-braces

set LDFLAGS= ^
    -luser32 ^
    -lgdi32 ^
    -lwinmm ^
    -lopengl32 ^
    -lshell32 ^
    -llib/glfw_windows/lib-vc2019/glfw3.lib

set RELCFLAGS= ^
    -O2 ^
    -Os

IF NOT EXIST ".\out" mkdir ".\out"
IF NOT EXIST ".\out\release" mkdir ".\out\release"

clang ^
    %CFLAGS% ^
    %RELCFLAGS% ^
    src\platform_windows.c ^
    -o .\out\release\transparent-cube.exe ^
    %LDFLAGS%
