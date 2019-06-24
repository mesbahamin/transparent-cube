struct Shader
{
    u32 program;
};

bool shader_compile(const GLchar *vertex_shader_source, const GLchar *fragment_shader_source, struct Shader *compiled_shader);
void shader_use(struct Shader *s);
void shader_setb(struct Shader *s, char *name, bool value);
void shader_seti(struct Shader *s, char *name, int value);
void shader_setf(struct Shader *s, char *name, f32 value);
void shader_setm4(struct Shader *s, char *name, m4 *mat);
void shader_setf3(struct Shader *s, char *name, f32 x, f32 y, f32 z);
void shader_setf3_1(struct Shader *s, char *name, f32 f);
void shader_setv3(struct Shader *s, char *name, v3 *v);
