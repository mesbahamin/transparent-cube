#include "platform_emscripten.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <emscripten.h>

#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>


void error_callback(int error, const char *description);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void platform_main_loop(void *data);


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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(PLATFORM_SCR_WIDTH, PLATFORM_SCR_HEIGHT, "Quaternion Demo", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "GLFW window creation failed\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    struct GameState game_state = {0};
    game_init(&game_state, PLATFORM_SCR_WIDTH, PLATFORM_SCR_HEIGHT);

    struct LoopArgs loop_args = {
        .counter = 0,
        .previous_s = 0.0f,
        .lag = 0.0f,
        .game_state = &game_state,
        .window = window,
    };
    void *args = &loop_args;

    emscripten_set_main_loop_arg(platform_main_loop, args, 0, 1);

    game_cleanup(&game_state);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


void platform_main_loop(void *data)
{
    struct LoopArgs *args = (struct LoopArgs *)(data);

    // glfwGetTimerValue() is not currently implemented in Emscripten.
    double current_s = glfwGetTime();
    double elapsed_s = current_s - args->previous_s;
    args->previous_s = current_s;
    args->lag += elapsed_s;
    //printf("%f, %f\n", elapsed_s, args->lag);

    int32_t framebuffer_width = PLATFORM_SCR_WIDTH;
    int32_t framebuffer_height = PLATFORM_SCR_HEIGHT;
    glfwGetFramebufferSize(args->window, &framebuffer_width, &framebuffer_height);

    // TODO: args->lag just increases forever. Fix that.
    game_update_and_render(args->game_state, args->lag, framebuffer_width, framebuffer_height);

    glfwSwapBuffers(args->window);
    glfwPollEvents();
}


void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
