#include "game.c"
#include "platform.h"
#include "platform_wasm.h"

struct GameState g_game_state = {0};
i32 g_width = PLATFORM_SCR_WIDTH;
i32 g_height = PLATFORM_SCR_HEIGHT;
char g_mem_buffer[1000] = {0};
i32 g_mem_buffer_i = 0;
u32 time = 0;

bool init(void)
{
    g_game_state.platform.platform_read_entire_file = &wasm_read_entire_file;
    g_game_state.platform.platform_print = &wasm_print;
    g_game_state.platform.platform_memory_free = &wasm_memory_free;
    game_init(&g_game_state, g_width, g_height);
    return true;
}

void render(void)
{
    time += 16;
    game_update_and_render(&g_game_state, time/1000.0f, g_width, g_height);
}

void window_resize(int w, int h)
{
    g_width = w;
    g_height = h;
    glViewport(0, 0, g_width, g_height);
}

i32 str_length(const char *str)
{
    i32 i = 0;
    while(str[i] != '\0')
    {
        i++;
    }
    return i;
}

void *memset(void *s, int c, size_t n)
{
    u8 *p = s;
    while(n > 0)
    {
        *p = (u8)c;
        p++;
        n--;
    }
    return s;
}

PLATFORM_READ_ENTIRE_FILE(wasm_read_entire_file)
{
    g_mem_buffer_i++;
    char *buf = &g_mem_buffer[g_mem_buffer_i];
    if(js_read_entire_file((i32)file_path, str_length(file_path), buf))
    {
        i32 file_data_length = str_length(buf);
        g_mem_buffer_i += file_data_length;
        g_mem_buffer[g_mem_buffer_i] = '\0';
        wasm_print("Succeeded in reading file.");
    }
    else
    {
        wasm_print("Failed to read file.");
    }
    return buf;
}

PLATFORM_PRINT(wasm_print)
{
    i32 len = str_length(format);
    js_print((i32)format, len);
    return 0;
}

PLATFORM_MEMORY_FREE(wasm_memory_free)
{
    g_mem_buffer_i = 0;
}
