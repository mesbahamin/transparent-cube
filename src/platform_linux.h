#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#include <stdbool.h>
#include <time.h>

#define PLATFORM_SCR_WIDTH 600
#define PLATFORM_SCR_HEIGHT 600

#define PLATFORM_SECOND 1000.0f
#define PLATFORM_FPS 60
#define PLATFORM_MS_PER_FRAME (PLATFORM_SECOND / PLATFORM_FPS)
#define PLATFORM_UPDATES_PER_SECOND 120
#define PLATFORM_MS_PER_UPDATE (PLATFORM_SECOND / PLATFORM_UPDATES_PER_SECOND)

PLATFORM_READ_ENTIRE_FILE(linux_read_entire_file);
PLATFORM_PRINT(linux_print);
PLATFORM_MEMORY_FREE(linux_memory_free);

#ifdef PLATFORM_HOTLOAD_GAME_CODE
#define PLATFORM_GAME_LIB_PATH "./out/release/game.so"
#endif // PLATFORM_HOTLOAD_GAME_CODE

#endif // PLATFORM_LINUX_H
