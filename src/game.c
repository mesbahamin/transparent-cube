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
    // load triangle vertex data
    {
        float circumradius = 10.0f;
        float inradius = circumradius * sinf(glmth_rad(30.0f));
        float half_side_length = circumradius * cosf(glmth_rad(30.0f));

        GLfloat triangle_vertices[] = {
            // positions                       // colors
            -half_side_length,     -inradius,  1.0f,  0.0f,  0.0f,
             half_side_length,     -inradius,  0.0f,  1.0f,  0.0f,
                         0.0f,  circumradius,  0.0f,  0.0f,  1.0f,
        };

        GLuint triangle_vao_id;
        GLuint triangle_vbo_id;
        glGenVertexArrays(1, &triangle_vao_id);
        glGenBuffers(1, &triangle_vbo_id);

        glBindVertexArray(triangle_vao_id);
        glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(*triangle_vertices), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // colors
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(*triangle_vertices), (GLvoid*)(2 * sizeof(*triangle_vertices)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

        game_state->triangle_vao_id = triangle_vao_id;
        game_state->triangle_vbo_id = triangle_vbo_id;
    }

    game_state->triangle_shader = shader_compile("shader/triangle_v.glsl", "shader/triangle_f.glsl");
}


void game_update_and_render(struct GameState *game_state, float dt, uint32_t screen_width, uint32_t screen_height)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render triangle
    {
        m4 model = glmth_m4_init_id();
        model = glmth_translate(model, glmth_v3_init(screen_width / 2.0f, screen_height / 2.0f, 0.0f));
        model = glmth_translate(model, glmth_v3_init(100.0f * cosf(dt), 100.0f * sinf(dt), 0.0f));
        model = glmth_rotate_z(model, -dt);
        model = glmth_scale(model, glmth_v3_init(10.0f, 10.0f, 10.0f));
        float scale_factor = fabs(1.0f * sinf(dt));
        model = glmth_scale(model, glmth_v3_init(scale_factor, scale_factor, scale_factor));

        shader_use(&game_state->triangle_shader);
        shader_setm4(&game_state->triangle_shader, "model", &model);

        m4 projection = glmth_m4_init_id();
        projection = glmth_projection_ortho(0.0f, screen_width, screen_height, 0.0f, -1.0f, 1.0f);
        shader_setm4(&game_state->triangle_shader, "projection", &projection);

        glBindVertexArray(game_state->triangle_vao_id);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }
}


void game_cleanup(struct GameState *game_state)
{
    glDeleteVertexArrays(1, &game_state->triangle_vao_id);
    glDeleteBuffers(1, &game_state->triangle_vbo_id);
}
