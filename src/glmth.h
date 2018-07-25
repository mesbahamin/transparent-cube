#ifndef GLMTH_H
#define GLMTH_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef float f32;
typedef double r64;

typedef union
{
    struct
    {
        f32 x, y;
    };
    f32 E[2];
} v2;

typedef union
{
    struct
    {
        f32 x, y, z;
    };
    struct
    {
        v2 xy;
        f32 ignored_z;
    };
    struct
    {
        f32 ignored_x;
        v2 yz;
    };
    f32 E[3];
} v3;

typedef union
{
    struct
    {
        union
        {
            v3 xyz;
            struct
            {
                f32 x, y, z;
            };
        };
        f32 w;
    };
    struct
    {
        v2 xy;
        f32 ignored_z_xy;
        f32 ignored_w_xy;
    };
    struct
    {
        f32 ignored_x_yz;
        v2 yz;
        f32 ignored_w_yz;
    };
    struct
    {
        f32 ignored_x_zw;
        f32 ignored_y_zw;
        v2 zw;
    };
    f32 E[4];
} v4;

typedef struct
{
    // row-major, so you probably want to transpose before passing to opengl,
    // which uses column-major matrices
    f32 E[4][4]; // E[row][column]
} m4;


m4 glmth_m4_init_id();
void glmth_m4_print(m4 m);
f32 *glmth_m4_valueptr(m4 m);
bool glmth_m4m4_eq(m4 mat1, m4 mat2);
m4 glmth_m4m4_m(m4 mat1, m4 mat2);
v4 glmth_m4v4_m(m4 m, v4 v);
v3 glmth_v3_cross(v3 vec1, v3 vec2);
v3 glmth_v3_init(f32 x, f32 y, f32 z);
v3 glmth_v3_init_f(f32 f);
f32 glmth_v3_length(v3 v);
v3 glmth_v3f_m(v3 v, f32 s);
v3 glmth_v3_negate(v3 v);
v3 glmth_v3_normalize(v3 v);
void glmth_v3_print(v3 v);
v3 glmth_v3_a(v3 vec1, v3 vec2);
v3 glmth_v3_s(v3 vec1, v3 vec2);
void glmth_v4_print(v4 v);
bool glmth_v4v4_eq(v4 vec1, v4 vec2);
void glmth_clampf(float *f, float min, float max);
f32 glmth_deg(f32 rad);
float glmth_lerpf(float f, float min, float max);
f32 glmth_rad(f32 deg);
m4 glmth_rotate_x(m4 m, f32 rad);
m4 glmth_rotate_y(m4 m, f32 rad);
m4 glmth_rotate_z(m4 m, f32 rad);
m4 glmth_rotate(m4 m, f32 rad, v3 axis);
m4 glmth_scale(m4 m, v3 v);
m4 glmth_translate(m4 m, v3 v);
m4 glmth_projection_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
m4 glmth_projection_perspective(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
m4 glmth_projection_perspective_fov(f32 fovy, f32 aspect, f32 near, f32 far);
m4 glmth_camera_look_at(v3 camera_pos, v3 camera_target, v3 up);

#endif
