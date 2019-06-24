#pragma once

typedef int    i32;
typedef float  f32;
typedef double f64;

// NOTE(amin): Since these functions will be implemented in javascript, we can
// only use i32, f32, and f64 params.
void webglAttachShader(i32 program, i32 shader);
void webglBindBuffer(i32 target, i32 buffer);
void webglBindVertexArray(i32 vao);
void webglBlendColor(f32 r, f32 g, f32 b, f32 a);
void webglBlendFunc(i32 sfactor, i32 dfactor);
void webglBufferData(i32 target, i32 size, i32 data, i32 usage);
void webglClear(i32 mask);
void webglClearColor(f32 r, f32 g, f32 b, f32 a);
void webglCompileShader(i32 shader);
i32  webglCreateBuffer(void);
i32  webglCreateProgram(void);
i32  webglCreateShader(i32 type);
i32  webglCreateVertexArray(void);
void webglDeleteBuffer(i32 bo);
void webglDeleteShader(i32 shader);
void webglDeleteVertexArray(i32 vao);
void webglDepthMask(i32 flag);
void webglDisable(i32 cap);
void webglDrawElements(i32 mode, i32 count, i32 type, i32 offset);
void webglEnable(i32 cap);
void webglEnableVertexAttribArray(i32 index);
void webglGetProgramInfoLog(void);
int  webglGetProgramParameter(i32 program, i32 param);
void webglGetShaderInfoLog(i32 shader, char *out_buf);
int  webglGetShaderParameter(i32 shader, i32 param);
i32  webglGetUniformLocation(i32 program, const char name[static 1], i32 name_len);
void webglLinkProgram(i32 program);
void webglShaderSource(i32 shader, const char source[static 1], i32 source_len);
void webglUniform1f(i32 location, f32 value);
void webglUniform1i(i32 location, i32 value);
void webglUniform3f(i32 location, f32 x, f32 y, f32 z);
void webglUniformMatrix4fv(i32 location, i32 transpose, const f32 data[static 16]);
void webglUseProgram(i32 program);
void webglVertexAttribPointer(i32 index, i32 size, i32 type, i32 normalized, i32 stride, i32 offset);
void webglViewport(i32 x, i32 y, i32 width, i32 height);

#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_TRIANGLES 0x0004
#define GL_SRC_ALPHA 0x0302
#define GL_DEPTH_TEST 0x0B71
#define GL_BLEND 0x0BE2
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

typedef unsigned int  GLenum;
typedef unsigned char GLboolean;
typedef unsigned int  GLbitfield;
typedef void          GLvoid;
typedef int           GLint;
typedef unsigned int  GLuint;
typedef int           GLsizei;
typedef float         GLfloat;
typedef char          GLchar;
typedef long          GLsizeiptr;

// TODO: add a version with safety checks
#define WEBGL_CAST_I32(x) (i32)(x)

static inline i32 _webgl_strlen(const char *str)
{
    i32 len = 0;
    while(str[len] != '\0')
    {
        len++;
    }
    return len;
}

inline void glAttachShader(GLuint program, GLuint shader)
{
    webglAttachShader(WEBGL_CAST_I32(program), WEBGL_CAST_I32(shader));
}

inline void glBindBuffer(GLenum target, GLuint buffer)
{
    webglBindBuffer(WEBGL_CAST_I32(target), WEBGL_CAST_I32(buffer));
}

inline void glBindVertexArray(GLuint array)
{
    webglBindVertexArray(WEBGL_CAST_I32(array));
}

inline void glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    webglBlendColor((f32)red, (f32)green, (f32)blue, (f32)alpha);
}

inline void glBlendFunc(GLenum sfactor, GLenum dfactor)
{
    webglBlendFunc(WEBGL_CAST_I32(sfactor), WEBGL_CAST_I32(dfactor));
}

inline void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    webglBufferData(WEBGL_CAST_I32(target), WEBGL_CAST_I32(size), WEBGL_CAST_I32(data), WEBGL_CAST_I32(usage));
}

inline void glClear(GLbitfield mask)
{
    webglClear(WEBGL_CAST_I32(mask));
}

inline void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    webglClearColor((f32)red, (f32)green, (f32)blue, (f32)alpha);
}

inline void glCompileShader(GLuint shader)
{
    webglCompileShader(WEBGL_CAST_I32(shader));
}

inline GLuint glCreateProgram(void)
{
    i32 program_id = webglCreateProgram();
    return (GLuint)program_id;
}

inline GLuint glCreateShader(GLenum type)
{
    return webglCreateShader(WEBGL_CAST_I32(type));
}

inline void glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    assert(n == 1);
    i32 the_buffer = WEBGL_CAST_I32(buffers[0]);
    webglDeleteBuffer(the_buffer);
}

inline void glDeleteShader(GLuint shader)
{
    webglDeleteShader(WEBGL_CAST_I32(shader));
}

inline void glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
    assert(n == 1);
    i32 the_array = WEBGL_CAST_I32(arrays[0]);
    webglDeleteVertexArray(the_array);
}

inline void glDepthMask(GLboolean flag)
{
    webglDepthMask(WEBGL_CAST_I32(flag));
}

inline void glDisable(GLenum cap)
{
    webglDisable(WEBGL_CAST_I32(cap));
}

inline void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
    webglDrawElements(WEBGL_CAST_I32(mode), WEBGL_CAST_I32(count), WEBGL_CAST_I32(type), WEBGL_CAST_I32(indices));
}

inline void glEnable(GLenum cap)
{
    webglEnable(WEBGL_CAST_I32(cap));
}

inline void glEnableVertexAttribArray(GLuint index)
{
    webglEnableVertexAttribArray(WEBGL_CAST_I32(index));
}

inline void glGenBuffers(GLsizei n, GLuint *buffers)
{
    assert(n == 1);
    i32 buffer_id = webglCreateBuffer();
    assert(buffer_id >= 0);
    *buffers = (GLuint)buffer_id;
}

inline void glGenVertexArrays(GLsizei n, GLuint *arrays)
{
    assert(n == 1);
    i32 vao_id = webglCreateVertexArray();
    assert(vao_id >= 0);
    *arrays = (GLuint)vao_id;
}

inline void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    // TODO: implement
    //webglGetProgramInfoLog(WEBGL_CAST_I32(program), WEBGL_CAST_I32(bufsize), WEBGL_CAST_I32(*length), WEBGL_CAST_I32(*infoLog));
}

inline void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    *params = webglGetProgramParameter(WEBGL_CAST_I32(program), WEBGL_CAST_I32(pname));
}

inline void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    webglGetShaderInfoLog(WEBGL_CAST_I32(shader), infoLog);
}

inline void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    *params = webglGetShaderParameter(WEBGL_CAST_I32(shader), WEBGL_CAST_I32(pname));
}

inline GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
    i32 name_len = _webgl_strlen(name);
    return webglGetUniformLocation(WEBGL_CAST_I32(program), name, name_len);
}

inline void glLinkProgram(GLuint program)
{
    webglLinkProgram(WEBGL_CAST_I32(program));
}

inline void glShaderSource(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length)
{
    const GLchar *s = (void *)*string;
    i32 l = _webgl_strlen(s);
    webglShaderSource(WEBGL_CAST_I32(shader), s, l);
}

inline void glUniform1f(GLint location, GLfloat v0)
{
    webglUniform1f(WEBGL_CAST_I32(location), (f32)v0);
}

inline void glUniform1i(GLint location, GLint v0)
{
    webglUniform1i(WEBGL_CAST_I32(location), WEBGL_CAST_I32(v0));
}

inline void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    webglUniform3f(WEBGL_CAST_I32(location), (f32)v0, (f32)v1, (f32)v2);
}

inline void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    webglUniformMatrix4fv(WEBGL_CAST_I32(location), WEBGL_CAST_I32(transpose), value);
}

inline void glUseProgram(GLuint program)
{
    webglUseProgram(WEBGL_CAST_I32(program));
}

inline void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    webglVertexAttribPointer(WEBGL_CAST_I32(index), WEBGL_CAST_I32(size), WEBGL_CAST_I32(type), WEBGL_CAST_I32(normalized), WEBGL_CAST_I32(stride), WEBGL_CAST_I32(pointer));
}

inline void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    webglViewport(x, y, WEBGL_CAST_I32(width), WEBGL_CAST_I32(height));
}

#undef WEBGL_CAST_I32
