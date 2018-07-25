#include "game.h"

#include <assert.h>
#include <math.h>

#include <glad/glad.h>


#ifdef PLATFORM_HOTLOAD_GAME_CODE
void game_load_opengl_symbols(void)
{
    gladLoadGL();
}
#endif


float wrap(float n, float min, float max)
{
    if (n > max)
    {
        return min;
    }
    else if (n < min)
    {
        return max;
    }
    else
    {
        return n;
    }
}


float randf(float min, float max)
{
    assert(min < max);
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}


void game_init(struct GameState *game_state, uint32_t screen_width, uint32_t screen_height)
{
    glEnable(GL_DEPTH_TEST);

    // load pyramid vertex data
    {
        float edge_length = 1.0f;
        float height = 1.0f / sqrtf(2.0f) * edge_length;
        float half_length = edge_length / 2.0f;
        float half_height = height / 2.0f;


        GLfloat pyramid_vertices[] = {
             // positions
             -half_length, -half_height, -half_length,
              half_length, -half_height, -half_length,
              half_length, -half_height,  half_length,

              half_length, -half_height,  half_length,
             -half_length, -half_height,  half_length,
             -half_length, -half_height, -half_length,

             -half_length, -half_height, -half_length,
              0.0f,         half_height,  0.0f,
              half_length, -half_height, -half_length,

              half_length, -half_height, -half_length,
              0.0f,         half_height,  0.0f,
              half_length, -half_height,  half_length,

              half_length, -half_height,  half_length,
              0.0f,         half_height,  0.0f,
             -half_length, -half_height,  half_length,

             -half_length, -half_height,  half_length,
              0.0f,         half_height,  0.0f,
             -half_length, -half_height, -half_length,
        };

        GLuint pyramid_vao_id;
        GLuint pyramid_vbo_id;
        glGenVertexArrays(1, &pyramid_vao_id);
        glGenBuffers(1, &pyramid_vbo_id);

        glBindVertexArray(pyramid_vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(*pyramid_vertices), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        game_state->pyramid_vao_id = pyramid_vao_id;
        game_state->pyramid_vbo_id = pyramid_vbo_id;
    }

    game_state->pyramid_shader = shader_compile("shader/pyramid_v.glsl", "shader/pyramid_f.glsl");
}


void game_update_and_render(struct GameState *game_state, float dt, uint32_t screen_width, uint32_t screen_height)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader_use(&game_state->pyramid_shader);

    m4 view = glmth_m4_init_id();
    m4 projection = glmth_m4_init_id();
    view = glmth_translate(view, glmth_v3_init(0.0f, 0.0f, -3.0f));
    projection = glmth_projection_perspective_fov(glmth_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

    shader_setm4(&game_state->pyramid_shader, "view", &view);
    shader_setm4(&game_state->pyramid_shader, "projection", &projection);

    // render pyramid
    {
        m4 model = glmth_m4_init_id();
        f32 angle = 20.0f;
        model = glmth_rotate(model, dt * glmth_rad(angle), glmth_v3_init(0.5f, 1.0f, 0.0f));

        f32 color_freq = dt * 0.1f;
        v3 pyramid_color = glmth_v3_init(
                sinf(color_freq),
                sinf(color_freq + (2 * M_PI / 3)),
                sinf(color_freq + (4 * M_PI / 3)));
        shader_setm4(&game_state->pyramid_shader, "model", &model);
        shader_setv3(&game_state->pyramid_shader, "pyramid_color", &pyramid_color);

        glBindVertexArray(game_state->pyramid_vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);
    }
}


void game_cleanup(struct GameState *game_state)
{
    glDeleteVertexArrays(1, &game_state->pyramid_vao_id);
    glDeleteBuffers(1, &game_state->pyramid_vbo_id);
}
