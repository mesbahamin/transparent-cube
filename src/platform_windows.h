#ifndef PLATFORM_WINDOWS_H
#define PLATFORM_WINDOWS_H

#include <stdbool.h>
#include <time.h>

#define PLATFORM_SCR_WIDTH 600
#define PLATFORM_SCR_HEIGHT 600

#define PLATFORM_SECOND 1000.0f
#define PLATFORM_FPS 60
#define PLATFORM_MS_PER_FRAME (PLATFORM_SECOND / PLATFORM_FPS)
#define PLATFORM_UPDATES_PER_SECOND 120
#define PLATFORM_MS_PER_UPDATE (PLATFORM_SECOND / PLATFORM_UPDATES_PER_SECOND)

PLATFORM_READ_ENTIRE_FILE(windows_read_entire_file);
PLATFORM_PRINT(windows_print);
PLATFORM_MEMORY_FREE(windows_memory_free);

#endif // PLATFORM_WINDOWS_H
