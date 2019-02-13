#include "platform_linux.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <dlfcn.h>
#include <sys/stat.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#ifdef PLATFORM_HOTLOAD_GAME_CODE
time_t file_get_modified_time(char *file_path);
void unload_game_code(struct GameCode *game_code);
struct GameCode load_game_code(char *source_lib_path);
#endif // PLATFORM_HOTLOAD_GAME_CODE

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
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
    game_init(&game_state, PLATFORM_SCR_WIDTH, PLATFORM_SCR_HEIGHT);

#ifdef PLATFORM_HOTLOAD_GAME_CODE
    struct GameCode game_code = load_game_code(PLATFORM_GAME_LIB_PATH);
    game_code.game_load_opengl_symbols();
#endif // PLATFORM_HOTLOAD_GAME_CODE

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

#ifdef PLATFORM_HOTLOAD_GAME_CODE
        time_t last_write_time = file_get_modified_time(PLATFORM_GAME_LIB_PATH);
        bool game_code_has_changed = last_write_time && (last_write_time != game_code.last_write_time);
        if (game_code_has_changed)
        {
            unload_game_code(&game_code);
            game_code = load_game_code(PLATFORM_GAME_LIB_PATH);
            game_code.game_load_opengl_symbols();
            if (!game_code.is_valid)
            {
                // TODO: fall back to backup?
            }
        }
        game_code.game_update_and_render(&game_state, lag/PLATFORM_SECOND, framebuffer_width, framebuffer_height);
#else
        game_update_and_render(&game_state, lag/PLATFORM_SECOND, framebuffer_width, framebuffer_height);
#endif // PLATFORM_HOTLOAD_GAME_CODE

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


time_t file_get_modified_time(char *file_path)
{
    time_t mtime = 0;
    struct stat file_status = {0};
    if (stat(file_path, &file_status) == 0)
    {
        //printf("File: %s last modified: %s\n", file_path, ctime(&file_status.st_mtime));
        mtime = file_status.st_mtime;
    }
    else
    {
        fprintf(stderr, "ERROR: Failed to stat file: %s\n", file_path);
    }
    return mtime;
}


#ifdef PLATFORM_HOTLOAD_GAME_CODE
void unload_game_code(struct GameCode *game_code)
{
    if (!game_code)
    {
        fprintf(stderr, "ERROR: Invalid pointer *game_code\n");
        return;
    }

    if (game_code->game_code_library)
    {
        dlclose(game_code->game_code_library);
        game_code->game_code_library = NULL;
    }
    game_code->is_valid = false;
    game_code->game_load_opengl_symbols = NULL;
    game_code->game_update_and_render = NULL;
}


// TODO: Add backup dll in case loading fails
struct GameCode load_game_code(char *source_lib_path)
{
    struct GameCode game_code = {0};

    game_code.last_write_time = file_get_modified_time(source_lib_path);
    if (game_code.last_write_time)
    {
        game_code.game_code_library = dlopen(source_lib_path, RTLD_LAZY);
        if (game_code.game_code_library)
        {
            // NOTE: The C standard (as of C99) distinguishes function pointers
            // from object pointers (`void *` is an object pointer). Technically it
            // is undefined behavior to cast between these two pointer types. In
            // practice, it works on most modern platforms.
            //
            // In this case, we are protected by POSIX, which specifies that
            // function and data pointers must be the same size. We will only ever
            // be using dlsym on POSIX-compliant platforms.
            game_code.game_load_opengl_symbols = (game_load_opengl_symbols_func *) dlsym(game_code.game_code_library, "game_load_opengl_symbols");
            game_code.game_update_and_render = (game_update_and_render_func *) dlsym(game_code.game_code_library, "game_update_and_render");
            game_code.is_valid = (game_code.game_load_opengl_symbols && game_code.game_update_and_render);
        }
    }

    if (!game_code.is_valid)
    {
        fprintf(stderr, "ERROR: Game code is not valid: %s\n", dlerror());
    }

    return game_code;
}
#endif // PLATFORM_HOTLOAD_GAME_CODE
