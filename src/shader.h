enum ShaderUniform
{
    SHADER_UNIFORM_MODEL,
    SHADER_UNIFORM_VIEW,
    SHADER_UNIFORM_PROJECTION,
    SHADER_UNIFORM_ALPHA,

    NUM_SHADER_UNIFORMS,
};

struct Shader
{
    u32 program;
    i32 uniform_locations[NUM_SHADER_UNIFORMS];
};

bool shader_compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source, struct Shader *compiled_shader);
void shader_use(struct Shader *s);
void shader_setb(struct Shader *s, enum ShaderUniform u, bool value);
void shader_seti(struct Shader *s, enum ShaderUniform u, int value);
void shader_setf(struct Shader *s, enum ShaderUniform u, f32 value);
void shader_setm4(struct Shader *s, enum ShaderUniform u, m4 *mat);
void shader_setf3(struct Shader *s, enum ShaderUniform u, f32 x, f32 y, f32 z);
void shader_setf3_1(struct Shader *s, enum ShaderUniform u, f32 f);
void shader_setv3(struct Shader *s, enum ShaderUniform u, v3 *v);
char *shader_uniform_get_name(enum ShaderUniform u);
