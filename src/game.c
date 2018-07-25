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

    // load cube vertex data
    {
        GLfloat cube_vertices[] = {
             // positions
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        GLuint cube_vao_id;
        GLuint cube_vbo_id;
        glGenVertexArrays(1, &cube_vao_id);
        glGenBuffers(1, &cube_vbo_id);

        glBindVertexArray(cube_vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(*cube_vertices), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        game_state->cube_vao_id = cube_vao_id;
        game_state->cube_vbo_id = cube_vbo_id;
    }

    game_state->cube_shader = shader_compile("shader/cube_v.glsl", "shader/cube_f.glsl");
}


void game_update_and_render(struct GameState *game_state, float dt, uint32_t screen_width, uint32_t screen_height)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader_use(&game_state->cube_shader);

    m4 view = glmth_m4_init_id();
    m4 projection = glmth_m4_init_id();
    view = glmth_translate(view, glmth_v3_init(0.0f, 0.0f, -3.0f));
    projection = glmth_projection_perspective_fov(glmth_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

    shader_setm4(&game_state->cube_shader, "view", &view);
    shader_setm4(&game_state->cube_shader, "projection", &projection);

    // render cube
    {
        m4 model = glmth_m4_init_id();
        f32 angle = 20.0f;
        model = glmth_rotate(model, dt * glmth_rad(angle), glmth_v3_init(0.5f, 1.0f, 0.0f));

        f32 color_freq = dt * 0.1f;
        v3 cube_color = glmth_v3_init(
                sinf(color_freq),
                sinf(color_freq + (2 * M_PI / 3)),
                sinf(color_freq + (4 * M_PI / 3)));
        shader_setm4(&game_state->cube_shader, "model", &model);
        shader_setv3(&game_state->cube_shader, "cube_color", &cube_color);

        glBindVertexArray(game_state->cube_vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
}


void game_cleanup(struct GameState *game_state)
{
    glDeleteVertexArrays(1, &game_state->cube_vao_id);
    glDeleteBuffers(1, &game_state->cube_vbo_id);
}
