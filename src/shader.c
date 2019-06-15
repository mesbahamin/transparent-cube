struct Shader shader_compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source)
{
    GLint success;
    GLchar info_log[512];

    if (!(vertex_shader_source && fragment_shader_source))
    {
        printf("Error: One or more shader source files weren't loaded.\n");
        exit(1);
    }
    else
    {
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", info_log);
            // TODO: handle errors here in a better way
            exit(1);
        }

        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", info_log);
            // TODO: handle errors here in a better way
            exit(1);
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
            // TODO: handle errors here in a better way
            exit(1);
        }

        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);

        return s;
    }
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
        f32 valueptr[sizeof(m4)];
        glmth_m4_valueptr(*mat, valueptr);
        glUniformMatrix4fv(glGetUniformLocation(s->program, name), 1, GL_TRUE, valueptr);
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
