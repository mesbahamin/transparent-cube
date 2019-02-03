#include "platform_linux.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


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
