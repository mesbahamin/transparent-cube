#define PLATFORM_SCR_WIDTH 600
#define PLATFORM_SCR_HEIGHT 600

PLATFORM_READ_ENTIRE_FILE(wasm_read_entire_file);
PLATFORM_PRINT(wasm_print);
PLATFORM_MEMORY_FREE(wasm_memory_free);

// Functions implemented in the JS loader
char *js_read_entire_file(i32 file_path, i32 name_len, char *file_data);
int js_print(i32 string, i32 len);
