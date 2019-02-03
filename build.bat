@echo off

set CFLAGS=-std=c99 -nostdlib -Ilib -I../include/glfw/include -Wall -Wextra -Wshadow -Wswitch-enum -Wno-unused-parameter -Wno-missing-braces
set LDFLAGS=-luser32 -lgdi32 -lwinmm -lopengl32 -lshell32
set RELCFLAGS=-O2 -Os

IF NOT EXIST ".\build" mkdir ".\build"
clang %CFLAGS% %RELCFLAGS%^
 src\platform_windows.c^
 src\game.c^
 src\glad.c^
 src\glmth.c^
 src\shader.c^
 ..\include\glfw\lib-vc2015\glfw3.lib^
 -o .\build\quaternion-demo.exe %LDFLAGS%
