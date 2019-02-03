#include "game.h"

#include <assert.h>
#include <math.h>

// TODO: remove references to emscripten
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif


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
    // load pyramid vertex data
    {
        float edge_length = 1.0f;
        float height = 1.0f / sqrtf(2.0f) * edge_length;
        float half_length = edge_length / 2.0f;
        float half_height = height / 2.0f;

        GLfloat pyramid_vertices[] = {
             // positions
             -half_length, -half_height, -half_length,
             -half_length, -half_height,  half_length,
              half_length, -half_height,  half_length,
              half_length, -half_height, -half_length,
              0.0f,         half_height,  0.0f,
        };

        GLuint elements[] = {
            0, 3, 2,
            2, 1, 0,
            0, 4, 3,
            3, 4, 2,
            2, 4, 1,
            1, 4, 0,
        };

        GLuint pyramid_vao_id;
        glGenVertexArrays(1, &pyramid_vao_id);
        glBindVertexArray(pyramid_vao_id);

        GLuint pyramid_vbo_id;
        glGenBuffers(1, &pyramid_vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(*pyramid_vertices), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        GLuint pyramid_ebo_id;
        glGenBuffers(1, &pyramid_ebo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramid_ebo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        glBindVertexArray(0);

        game_state->pyramid_vao_id = pyramid_vao_id;
        game_state->pyramid_vbo_id = pyramid_vbo_id;
        game_state->pyramid_ebo_id = pyramid_ebo_id;
    }

    // load cube vertex data
    {
        GLfloat cube_vertices[] = {
             // positions         // colors
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        };

        GLuint elements[] = {
            0, 4, 6, 6, 2, 0,
            1, 5, 7, 7, 3, 1,
            3, 2, 0, 0, 1, 3,
            7, 6, 4, 4, 5, 7,
            0, 4, 5, 5, 1, 0,
            2, 6, 7, 7, 3, 2,
        };

        GLuint cube_vao;
        glGenVertexArrays(1, &cube_vao);
        glBindVertexArray(cube_vao);

        GLuint cube_vbo;
        glGenBuffers(1, &cube_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(*cube_vertices), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(*cube_vertices), (GLvoid*)(3 * sizeof(*cube_vertices)));
        glEnableVertexAttribArray(1);

        GLuint cube_ebo;
        glGenBuffers(1, &cube_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        glBindVertexArray(0);

        game_state->cube_vao = cube_vao;
        game_state->cube_vbo = cube_vbo;
        game_state->cube_ebo = cube_ebo;
    }

    game_state->pyramid_shader = shader_compile("shader/pyramid_v.glsl", "shader/pyramid_f.glsl");
    game_state->cube_shader = shader_compile("shader/cube_v.glsl", "shader/cube_f.glsl");

    glEnable(GL_DEPTH_TEST);

    // TODO: remove references to emscripten
#ifndef __EMSCRIPTEN__
    glEnable(GL_MULTISAMPLE);
#endif
}


void game_update_and_render(struct GameState *game_state, float dt, uint32_t screen_width, uint32_t screen_height)
{
    glDepthMask(GL_TRUE);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m4 view = glmth_m4_init_id();
    m4 projection = glmth_m4_init_id();
    view = glmth_translate(view, glmth_v3_init(0.0f, 0.0f, -3.0f));
    projection = glmth_projection_perspective_fov(glmth_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

#if 0
    // render pyramid
    {
        shader_use(&game_state->pyramid_shader);
        shader_setm4(&game_state->pyramid_shader, "view", &view);
        shader_setm4(&game_state->pyramid_shader, "projection", &projection);

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
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
#endif

    // render cube
    {
        // Depth testing causes the blending to be applied inconsistently
        // depending on the draw order, so we must disable it.
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        // TODO: get this to work regardless of background color
        glBlendColor(0.0f, 0.0f, 0.0f, 1.0f);
        glBlendFunc(GL_SRC_ALPHA, GL_CONSTANT_ALPHA);

        shader_use(&game_state->cube_shader);
        shader_setm4(&game_state->cube_shader, "view", &view);
        shader_setm4(&game_state->cube_shader, "projection", &projection);

        m4 model = glmth_m4_init_id();
        f32 angle = 20.0f;
        model = glmth_rotate(model, dt * glmth_rad(angle), glmth_v3_init(1.0f, 0.0f, 0.0f));
        model = glmth_rotate(model, dt * glmth_rad(angle), glmth_v3_init(0.0f, 1.0f, 0.0f));
        model = glmth_rotate(model, dt * glmth_rad(angle), glmth_v3_init(0.0f, 0.0f, 1.0f));

        f32 alpha = 0.2f * (1.5f + sinf(0.5f * dt));
        shader_setm4(&game_state->cube_shader, "model", &model);
        shader_setf(&game_state->cube_shader, "alpha", alpha);

        glBindVertexArray(game_state->cube_vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}


void game_cleanup(struct GameState *game_state)
{
    glDeleteVertexArrays(1, &game_state->pyramid_vao_id);
    glDeleteBuffers(1, &game_state->pyramid_vbo_id);
    glDeleteBuffers(1, &game_state->pyramid_ebo_id);
}
