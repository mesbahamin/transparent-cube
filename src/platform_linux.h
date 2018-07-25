#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#include <stdbool.h>
#include <time.h>

#include "game.h"

#define PLATFORM_SCR_WIDTH 600
#define PLATFORM_SCR_HEIGHT 600

#define PLATFORM_SECOND 1000.0f
#define PLATFORM_FPS 60
#define PLATFORM_MS_PER_FRAME (PLATFORM_SECOND / PLATFORM_FPS)
#define PLATFORM_UPDATES_PER_SECOND 120
#define PLATFORM_MS_PER_UPDATE (PLATFORM_SECOND / PLATFORM_UPDATES_PER_SECOND)

#ifdef PLATFORM_HOTLOAD_GAME_CODE
#define PLATFORM_GAME_LIB_PATH "./build/release/game.so"
struct GameCode
{
    bool is_valid;
    void *game_code_library;
    time_t last_write_time;
    game_load_opengl_symbols_func *game_load_opengl_symbols;
    game_update_and_render_func *game_update_and_render;
};
#endif // PLATFORM_HOTLOAD_GAME_CODE

#endif // PLATFORM_LINUX_H
