#pragma once

#define PLATFORM_READ_ENTIRE_FILE(name) char *(name)(char *file_path)
typedef PLATFORM_READ_ENTIRE_FILE(platform_read_entire_file_func);

#define PLATFORM_PRINT(name) int (name)(const char *format, ...)
typedef PLATFORM_PRINT(platform_print_func);

#define PLATFORM_MEMORY_FREE(name) void (name)(void *ptr)
typedef PLATFORM_MEMORY_FREE(platform_memory_free_func);

struct PlatformApi
{
    platform_read_entire_file_func* platform_read_entire_file;
    platform_print_func* platform_print;
    platform_memory_free_func* platform_memory_free;
};

struct GameState
{
    u32 cube_vao;
    u32 cube_vbo;
    u32 cube_ebo;
    struct Shader cube_shader;
    struct PlatformApi platform;
};

#ifdef PLATFORM_HOTLOAD_GAME_CODE
#include <time.h>
// We need to call this from the platform layer in order for the game, when
// built as a shared object library to have access to the OpenGL symbols.
// https://github.com/Dav1dde/glad/issues/151
typedef void (game_load_opengl_symbols_func)(void);

typedef void (game_update_and_render_func)(struct GameState *game_state, float dt, u32 screen_width, u32 screen_height);

struct GameCode
{
    bool is_valid;
    void *game_code_library;
    time_t last_write_time;
    game_load_opengl_symbols_func *game_load_opengl_symbols;
    game_update_and_render_func *game_update_and_render;
};
#endif // PLATFORM_HOTLOAD_GAME_CODE
