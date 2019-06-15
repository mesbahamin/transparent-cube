#pragma once

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef GAME_WEBGL
#include "webgl.h"
#else
#include "glad/glad.h"
#endif

#include "glmth.h"
#include "shader.h"

struct GameState
{
    u32 cube_vao;
    u32 cube_vbo;
    u32 cube_ebo;
    struct Shader cube_shader;
};

typedef void (game_update_and_render_func)(struct GameState *game_state, float dt, u32 screen_width, u32 screen_height);
void game_update_and_render(struct GameState *game_state, float dt, u32 screen_width, u32 screen_height);

void game_init(struct GameState *game_state, u32 screen_width, u32 screen_height);
void game_cleanup(struct GameState *game_state);

#ifdef PLATFORM_HOTLOAD_GAME_CODE
// We need to call this from the platform layer in order for the game, when
// built as a shared object library to have access to the OpenGL symbols.
// https://github.com/Dav1dde/glad/issues/151
typedef void (game_load_opengl_symbols_func)(void);
void game_load_opengl_symbols(void);
#endif // PLATFORM_HOTLOAD_GAME_CODE
