#include "shader.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif


char *read_file(char *file_path)
{
    FILE *handle = fopen(file_path, "r");
    char *buffer = NULL;

    if (handle)
    {
        // get file size
        fseek(handle, 0, SEEK_END);
        uint32_t num_bytes_in_file = ftell(handle);
        rewind(handle);

        buffer = (char*) malloc(sizeof(char) * (num_bytes_in_file + 1) );

        uint32_t bytes_read = fread(buffer, sizeof(char), num_bytes_in_file, handle);
        // IMPORTANT! fread() doesn't add the '\0'
        buffer[num_bytes_in_file] = '\0';

        if (num_bytes_in_file != bytes_read)
        {
            free(buffer);
            buffer = NULL;
        }

        fclose(handle);
    }
    else
    {
        printf("Error: Couldn't open file at path: %s", file_path);
    }

    return buffer;
}


struct Shader shader_compile(char *vertex_path, char *fragment_path)
{
    const GLchar *vertex_shader_source =
        "#version 300 es\n"
        "precision highp float;\n"
        "layout (location = 0) in vec3 a_position;\n"
        "layout (location = 1) in vec3 a_color;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * vec4(a_position, 1.0f);\n"
        "    color = a_color;\n"
        "}\n";

    const GLchar *fragment_shader_source =
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform float alpha;\n"
        "in vec3 color;\n"
        "out vec4 frag_color;\n"
        "void main()\n"
        "{\n"
        "    frag_color = vec4(color, alpha);\n"
        "}\n";

    GLint success;
    GLchar info_log[512];

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", info_log);
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", info_log);
    }

    struct Shader s;
    s.program = glCreateProgram();
    glAttachShader(s.program, vertex_shader);
    glAttachShader(s.program, fragment_shader);
    glLinkProgram(s.program);
    glGetProgramiv(s.program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(s.program, 512, NULL, info_log);
        printf("ERROR::SHADER::LINKING_FAILED\n %s\n", info_log);
    }

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    return s;
}


void shader_use(struct Shader *s)
{
    if (s)
    {
        glUseProgram(s->program);
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}


void shader_setb(struct Shader *s, char *name, bool value)
{
    if (s)
    {
        glUniform1i(glGetUniformLocation(s->program, name), (int)value);
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}


void shader_seti(struct Shader *s, char *name, int value)
{
    if (s)
    {
        glUniform1i(glGetUniformLocation(s->program, name), value);
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}


void shader_setf(struct Shader *s, char *name, f32 value)
{
    if (s)
    {
        glUniform1f(glGetUniformLocation(s->program, name), value);
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}


void shader_setm4(struct Shader *s, char *name, m4 *mat)
{
    if (s)
    {
        glUniformMatrix4fv(glGetUniformLocation(s->program, name), 1, GL_TRUE, glmth_m4_valueptr(*mat));
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}


void shader_setf3(struct Shader *s, char *name, f32 x, f32 y, f32 z)
{
    if (s)
    {
        glUniform3f(glGetUniformLocation(s->program, name), x, y, z);
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}


void shader_setf3_1(struct Shader *s, char *name, f32 f)
{
    if (s)
    {
        shader_setf3(s, name, f, f, f);
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}


void shader_setv3(struct Shader *s, char *name, v3 *v)
{
    if (s)
    {
        shader_setf3(s, name, v->x, v->y, v->z);
    }
    else
    {
        printf("Error: invalid Shader pointer\n");
    }
}
