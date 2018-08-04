#ifndef GAME_H
#define GAME_H

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#include "glmth.h"
#include "shader.h"


struct GameState
{
    GLuint pyramid_vao_id;
    GLuint pyramid_vbo_id;
    GLuint pyramid_ebo_id;
    struct Shader pyramid_shader;
};

#ifdef PLATFORM_HOTLOAD_GAME_CODE
// We need to call this from the platform layer in order for the game, when
// built as a shared object library to have access to the OpenGL symbols.
// https://github.com/Dav1dde/glad/issues/151
typedef void (game_load_opengl_symbols_func)(void);
void game_load_opengl_symbols(void);
#endif // PLATFORM_HOTLOAD_GAME_CODE

typedef void (game_update_and_render_func)(struct GameState *game_state, float dt, uint32_t screen_width, uint32_t screen_height);
void game_update_and_render(struct GameState *game_state, float dt, uint32_t screen_width, uint32_t screen_height);

void game_init(struct GameState *game_state, uint32_t screen_width, uint32_t screen_height);
void game_cleanup(struct GameState *game_state);

#endif
