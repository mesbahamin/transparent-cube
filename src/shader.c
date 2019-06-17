bool shader_compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source, struct Shader *compiled_shader)
{
    GLint success;
    GLchar info_log[512];

    if (!(vertex_shader_source && fragment_shader_source))
    {
        print("Error: One or more shader source files weren't loaded.\n");
        return false;
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
            print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", info_log);
            // TODO: handle errors here in a better way
            return false;
        }

        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
            print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", info_log);
            // TODO: handle errors here in a better way
            return false;
        }

        compiled_shader->program = glCreateProgram();
        glAttachShader(compiled_shader->program, vertex_shader);
        glAttachShader(compiled_shader->program, fragment_shader);
        glLinkProgram(compiled_shader->program);
        glGetProgramiv(compiled_shader->program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(compiled_shader->program, 512, NULL, info_log);
            print("ERROR::SHADER::LINKING_FAILED\n %s\n", info_log);
            // TODO: handle errors here in a better way
            return false;
        }

        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);

        return true;
    }
}

bool shader_check_pointer(struct Shader *s)
{
    if (s)
    {
        return true;
    }
    else
    {
        print("Error: invalid Shader pointer\n");
        return false;
    }
}

void shader_use(struct Shader *s)
{
    if (shader_check_pointer(s))
    {
        glUseProgram(s->program);
    }
}


void shader_setb(struct Shader *s, char *name, bool value)
{
    if (shader_check_pointer(s))
    {
        glUniform1i(glGetUniformLocation(s->program, name), (int)value);
    }
}


void shader_seti(struct Shader *s, char *name, int value)
{
    if (shader_check_pointer(s))
    {
        glUniform1i(glGetUniformLocation(s->program, name), value);
    }
}


void shader_setf(struct Shader *s, char *name, f32 value)
{
    if (shader_check_pointer(s))
    {
        glUniform1f(glGetUniformLocation(s->program, name), value);
    }
}


void shader_setm4(struct Shader *s, char *name, m4 *mat)
{
    if (shader_check_pointer(s))
    {
        f32 valueptr[sizeof(m4)];
        glmth_m4_valueptr(*mat, valueptr);
        glUniformMatrix4fv(glGetUniformLocation(s->program, name), 1, GL_TRUE, valueptr);
    }
}


void shader_setf3(struct Shader *s, char *name, f32 x, f32 y, f32 z)
{
    if (shader_check_pointer(s))
    {
        glUniform3f(glGetUniformLocation(s->program, name), x, y, z);
    }
}


void shader_setf3_1(struct Shader *s, char *name, f32 f)
{
    if (shader_check_pointer(s))
    {
        shader_setf3(s, name, f, f, f);
    }
}


void shader_setv3(struct Shader *s, char *name, v3 *v)
{
    if (shader_check_pointer(s))
    {
        shader_setf3(s, name, v->x, v->y, v->z);
    }
}
