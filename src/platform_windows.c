#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// LOL, Windef.h
#undef near
#undef far

#include "game.c"
#include "platform.h"
#include "platform_windows.h"

void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#ifdef GLAD_DEBUG
void pre_gl_callback(const char *func_name, void *func_ptr, int len_args, ...)
{
    printf("Calling: %s (%d arguments)\n", func_name, len_args);
}
#endif // GLAD_DEBUG


int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        fprintf(stderr, "GLFW initialization failed\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(PLATFORM_SCR_WIDTH, PLATFORM_SCR_HEIGHT, "Quaternion Demo", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "GLFW window creation failed\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        fprintf(stderr, "glad initialization failed\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

#ifdef USE_TEST_SEED
    srand((uint32_t)0);
#else
    srand((uint32_t)time(NULL));
#endif // USE_TEST_SEED

    struct GameState game_state = {0};
    game_state.platform.platform_read_entire_file = &windows_read_entire_file;
    game_state.platform.platform_print = &windows_print;
    game_state.platform.platform_memory_free = &windows_memory_free;

    game_init(&game_state, PLATFORM_SCR_WIDTH, PLATFORM_SCR_HEIGHT);

    uint64_t lag = 0;
    uint64_t previous_ms = (glfwGetTimerValue() * PLATFORM_SECOND) / glfwGetTimerFrequency();

    while (!glfwWindowShouldClose(window))
    {
        uint64_t current_ms = (glfwGetTimerValue() * PLATFORM_SECOND) / glfwGetTimerFrequency();
        uint64_t elapsed_ms = current_ms - previous_ms;
        previous_ms = current_ms;
        lag += elapsed_ms;
        //printf("%" PRIu64 ", %" PRIu64 ", %f\n", elapsed_ms, lag, PLATFORM_MS_PER_UPDATE);

        int32_t framebuffer_width = PLATFORM_SCR_WIDTH;
        int32_t framebuffer_height = PLATFORM_SCR_HEIGHT;
        glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

        game_update_and_render(&game_state, lag/PLATFORM_SECOND, framebuffer_width, framebuffer_height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    game_cleanup(&game_state);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

PLATFORM_MEMORY_FREE(windows_memory_free)
{
    free(ptr);
}

PLATFORM_PRINT(windows_print)
{
    va_list args;
    va_start(args, format);
    int num_chars_printed = vprintf(format, args);
    va_end(args);
    return num_chars_printed;
}

PLATFORM_READ_ENTIRE_FILE(windows_read_entire_file)
{
    FILE *handle = fopen(file_path, "rb");
    char *buffer = NULL;

    if (handle)
    {
        // get file size
        fseek(handle, 0, SEEK_END);
        u32 num_bytes_in_file = ftell(handle);
        rewind(handle);

        // TODO: replace malloc with own allocator so I stop having nightmares
        buffer = (char*) malloc(sizeof(char) * (num_bytes_in_file + 1) );

        u32 bytes_read = fread(buffer, sizeof(char), num_bytes_in_file, handle);
        // IMPORTANT! fread() doesn't add the '\0'
        buffer[num_bytes_in_file] = '\0';

        if (num_bytes_in_file != bytes_read)
        {
            free(buffer);
            buffer = NULL;
        }

        fclose(handle);
    }
    else
    {
        printf("Error: Couldn't open file at path: %s", file_path);
        // TODO: handle errors here in a better way
        exit(1);
    }

    return buffer;
}
