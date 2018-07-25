#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // get opengl headers
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "glmth.h"

struct Shader
{
    uint32_t program;
};

char *read_file(char *file_path);
struct Shader shader_compile(GLchar *vertex_path, GLchar *fragment_path);
void shader_use(struct Shader *s);
void shader_setb(struct Shader *s, char *name, bool value);
void shader_seti(struct Shader *s, char *name, int value);
void shader_setf(struct Shader *s, char *name, f32 value);
void shader_setm4(struct Shader *s, char *name, m4 *mat);
void shader_setf3(struct Shader *s, char *name, f32 x, f32 y, f32 z);
void shader_setf3_1(struct Shader *s, char *name, f32 f);
void shader_setv3(struct Shader *s, char *name, v3 *v);

#endif
