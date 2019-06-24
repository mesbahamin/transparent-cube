#ifndef PLATFORM_EMSCRIPTEN_H
#define PLATFORM_EMSCRIPTEN_H

#include <stdbool.h>
#include <time.h>

#define PLATFORM_SCR_WIDTH 600
#define PLATFORM_SCR_HEIGHT 600

struct LoopArgs
{
    int counter;
    struct GameState *game_state;
    double previous_s;
    double lag;
    GLFWwindow* window;
};

#endif // PLATFORM_EMSCRIPTEN_H
