#include "game.h"

#ifndef GAME_WEBGL
#include "glad.c"
#endif

#include "shader.c"

void game_init(struct GameState *game_state, u32 screen_width, u32 screen_height)
{
#if 0
    {
        GLfloat triangle_vertices[] = {
            // positions   // colors
            -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
        };
        u32 elements[] = {
            0, 1, 2,
        };

        GLuint VBO;
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // colors
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        u32 cube_ebo;
        glGenBuffers(1, &cube_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        game_state->cube_vao = VAO;
        game_state->cube_vbo = VBO;
        game_state->cube_ebo = cube_ebo;
    }
#else
    // load cube vertex data
    {
        f32 cube_vertices[] = {
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

        u32 elements[] = {
            0, 4, 6, 6, 2, 0,
            1, 5, 7, 7, 3, 1,
            3, 2, 0, 0, 1, 3,
            7, 6, 4, 4, 5, 7,
            0, 4, 5, 5, 1, 0,
            2, 6, 7, 7, 3, 2,
        };

        u32 cube_vao;
        glGenVertexArrays(1, &cube_vao);
        glBindVertexArray(cube_vao);

        u32 cube_vbo;
        glGenBuffers(1, &cube_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(*cube_vertices), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(*cube_vertices), (GLvoid*)(3 * sizeof(*cube_vertices)));
        glEnableVertexAttribArray(1);

        u32 cube_ebo;
        glGenBuffers(1, &cube_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


        game_state->cube_vao = cube_vao;
        game_state->cube_vbo = cube_vbo;
        game_state->cube_ebo = cube_ebo;
    }
#endif

    // game_shader_load
    {
        struct PlatformApi platform = game_state->platform;
        char *v_source = platform.platform_read_entire_file("shader/cube_v.glsl");
        char *f_source = platform.platform_read_entire_file("shader/cube_f.glsl");
        print = platform.platform_print;
        struct Shader main_shader = {0};
        // TODO: Check this result
        shader_compile(v_source, f_source, &main_shader);
        platform.platform_memory_free(v_source);
        platform.platform_memory_free(f_source);
        game_state->cube_shader = main_shader;
    }

    //glEnable(GL_DEPTH_TEST);

#ifndef GAME_WEBGL
    //glEnable(GL_MULTISAMPLE);
#endif
}

void game_update_and_render(struct GameState *game_state, float dt, u32 screen_width, u32 screen_height)
{
#if 0
    {
        m4 view = glmth_m4_init_id();
        m4 projection = glmth_m4_init_id();
        view = glmth_translate(view, glmth_v3_init(0.0f, 0.0f, -3.0f));
        projection = glmth_projection_perspective_fov(glmth_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

        glClearColor(0.1f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m4 model = glmth_m4_init_id();
        f32 angle = 20.0f;
        f32 rot_rad = dt * glmth_rad(angle);
        model = glmth_rotate(model, rot_rad, glmth_v3_init(0.0f, 0.0f, 1.0f));
        model = glmth_rotate(model, rot_rad, glmth_v3_init(0.0f, 1.0f, 0.0f));
        model = glmth_rotate(model, rot_rad, glmth_v3_init(1.0f, 0.0f, 0.0f));

        shader_use(&game_state->cube_shader);
        shader_setm4(&game_state->cube_shader, "model", &model);
        shader_setm4(&game_state->cube_shader, "view", &view);
        shader_setm4(&game_state->cube_shader, "projection", &projection);
        shader_setf(&game_state->cube_shader, "alpha", 1.0f);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    }
#else
    glDepthMask(GL_TRUE);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m4 view = glmth_m4_init_id();
    m4 projection = glmth_m4_init_id();
    //TODO: make these transforms work on webgl like the do natively
    //view = glmth_translate(view, glmth_v3_init(0.0f, 0.0f, -3.0f));
    //projection = glmth_projection_perspective_fov(glmth_rad(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

    // render cube
    {
        // Depth testing causes the blending to be applied inconsistently
        // depending on the draw order, so we must disable it.
        //glDisable(GL_DEPTH_TEST);

        //glEnable(GL_BLEND);
        // TODO: get this to work regardless of background color
        glBlendColor(0.0f, 0.0f, 0.0f, 1.0f);
        glBlendFunc(GL_SRC_ALPHA, GL_CONSTANT_ALPHA);

        shader_use(&game_state->cube_shader);
        shader_setm4(&game_state->cube_shader, "view", &view);
        shader_setm4(&game_state->cube_shader, "projection", &projection);

        m4 model = glmth_m4_init_id();
        f32 angle = 20.0f;
        // TODO: fmodf this so we don't get huge numbers eventually
        f32 rot_rad = dt * glmth_rad(angle);
        // TODO: Figure out why the cube rotates counter clockwise on linux but
        // clockwise on wasm
        //model = glmth_rotate(model, rot_rad, glmth_v3_init(1.0f, 0.0f, 0.0f));
        //model = glmth_rotate(model, rot_rad, glmth_v3_init(0.0f, 1.0f, 0.0f));
        model = glmth_rotate(model, rot_rad, glmth_v3_init(0.0f, 0.0f, 1.0f));

        f32 alpha = 0.2f * (1.5f + glmth_sinf(0.5f * dt));
        shader_setm4(&game_state->cube_shader, "model", &model);
        shader_setf(&game_state->cube_shader, "alpha", alpha);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
#endif
}

void game_cleanup(struct GameState *game_state)
{
    glDeleteVertexArrays(1, &game_state->cube_vao);
    glDeleteBuffers(1, &game_state->cube_vbo);
    glDeleteBuffers(1, &game_state->cube_ebo);
}

#ifdef PLATFORM_HOTLOAD_GAME_CODE
void game_load_opengl_symbols(void)
{
    gladLoadGL();
}
#endif
