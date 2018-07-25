#include "glmth.h"

#include <stdio.h>
#include <stdlib.h>

m4 glmth_m4_init_id()
{
    m4 m = { 0 };
    m.E[0][0] = 1.0f;
    m.E[1][1] = 1.0f;
    m.E[2][2] = 1.0f;
    m.E[3][3] = 1.0f;
    return m;
}


void glmth_m4_print(m4 m)
{
    printf("[\n");
    printf("  %f, %f, %f, %f\n", m.E[0][0], m.E[0][1], m.E[0][2], m.E[0][3]);
    printf("  %f, %f, %f, %f\n", m.E[1][0], m.E[1][1], m.E[1][2], m.E[1][3]);
    printf("  %f, %f, %f, %f\n", m.E[2][0], m.E[2][1], m.E[2][2], m.E[2][3]);
    printf("  %f, %f, %f, %f\n", m.E[3][0], m.E[3][1], m.E[3][2], m.E[3][3]);
    printf("]\n");
}


f32 *glmth_m4_valueptr(m4 m)
{
    f32 *values = malloc(sizeof(m4));
    for (u8 v = 0; v < 16; ++v)
    {
        u8 row = v / 4;
        u8 col = v % 4;
        values[v] = m.E[row][col];
    }
    return values;
}


bool glmth_m4m4_eq(m4 mat1, m4 mat2)
{
    for (u8 i = 0; i < 4; ++i)
    {
        for (u8 j = 0; j < 4; ++j)
        {
            if (mat1.E[i][j] != mat2.E[i][j])
            {
                return false;
            }
        }
    }
    return true;
}


m4 glmth_m4m4_m(m4 mat1, m4 mat2)
{
    m4 r = {
        .E[0][0] = (mat1.E[0][0] * mat2.E[0][0]) + (mat1.E[0][1] * mat2.E[1][0]) + (mat1.E[0][2] * mat2.E[2][0]) + (mat1.E[0][3] * mat2.E[3][0]),
        .E[0][1] = (mat1.E[0][0] * mat2.E[0][1]) + (mat1.E[0][1] * mat2.E[1][1]) + (mat1.E[0][2] * mat2.E[2][1]) + (mat1.E[0][3] * mat2.E[3][1]),
        .E[0][2] = (mat1.E[0][0] * mat2.E[0][2]) + (mat1.E[0][1] * mat2.E[1][2]) + (mat1.E[0][2] * mat2.E[2][2]) + (mat1.E[0][3] * mat2.E[3][2]),
        .E[0][3] = (mat1.E[0][0] * mat2.E[0][3]) + (mat1.E[0][1] * mat2.E[1][3]) + (mat1.E[0][2] * mat2.E[2][3]) + (mat1.E[0][3] * mat2.E[3][3]),

        .E[1][0] = (mat1.E[1][0] * mat2.E[0][0]) + (mat1.E[1][1] * mat2.E[1][0]) + (mat1.E[1][2] * mat2.E[2][0]) + (mat1.E[1][3] * mat2.E[3][0]),
        .E[1][1] = (mat1.E[1][0] * mat2.E[0][1]) + (mat1.E[1][1] * mat2.E[1][1]) + (mat1.E[1][2] * mat2.E[2][1]) + (mat1.E[1][3] * mat2.E[3][1]),
        .E[1][2] = (mat1.E[1][0] * mat2.E[0][2]) + (mat1.E[1][1] * mat2.E[1][2]) + (mat1.E[1][2] * mat2.E[2][2]) + (mat1.E[1][3] * mat2.E[3][2]),
        .E[1][3] = (mat1.E[1][0] * mat2.E[0][3]) + (mat1.E[1][1] * mat2.E[1][3]) + (mat1.E[1][2] * mat2.E[2][3]) + (mat1.E[1][3] * mat2.E[3][3]),

        .E[2][0] = (mat1.E[2][0] * mat2.E[0][0]) + (mat1.E[2][1] * mat2.E[1][0]) + (mat1.E[2][2] * mat2.E[2][0]) + (mat1.E[2][3] * mat2.E[3][0]),
        .E[2][1] = (mat1.E[2][0] * mat2.E[0][1]) + (mat1.E[2][1] * mat2.E[1][1]) + (mat1.E[2][2] * mat2.E[2][1]) + (mat1.E[2][3] * mat2.E[3][1]),
        .E[2][2] = (mat1.E[2][0] * mat2.E[0][2]) + (mat1.E[2][1] * mat2.E[1][2]) + (mat1.E[2][2] * mat2.E[2][2]) + (mat1.E[2][3] * mat2.E[3][2]),
        .E[2][3] = (mat1.E[2][0] * mat2.E[0][3]) + (mat1.E[2][1] * mat2.E[1][3]) + (mat1.E[2][2] * mat2.E[2][3]) + (mat1.E[2][3] * mat2.E[3][3]),

        .E[3][0] = (mat1.E[3][0] * mat2.E[0][0]) + (mat1.E[3][1] * mat2.E[1][0]) + (mat1.E[3][2] * mat2.E[2][0]) + (mat1.E[3][3] * mat2.E[3][0]),
        .E[3][1] = (mat1.E[3][0] * mat2.E[0][1]) + (mat1.E[3][1] * mat2.E[1][1]) + (mat1.E[3][2] * mat2.E[2][1]) + (mat1.E[3][3] * mat2.E[3][1]),
        .E[3][2] = (mat1.E[3][0] * mat2.E[0][2]) + (mat1.E[3][1] * mat2.E[1][2]) + (mat1.E[3][2] * mat2.E[2][2]) + (mat1.E[3][3] * mat2.E[3][2]),
        .E[3][3] = (mat1.E[3][0] * mat2.E[0][3]) + (mat1.E[3][1] * mat2.E[1][3]) + (mat1.E[3][2] * mat2.E[2][3]) + (mat1.E[3][3] * mat2.E[3][3]),
    };
    return r;
}


v4 glmth_m4v4_m(m4 m, v4 v)
{
    v4 r = {
        .x = (m.E[0][0] * v.x) + (m.E[0][1] * v.y) + (m.E[0][2] * v.z) + (m.E[0][3] * v.w),
        .y = (m.E[1][0] * v.x) + (m.E[1][1] * v.y) + (m.E[1][2] * v.z) + (m.E[1][3] * v.w),
        .z = (m.E[2][0] * v.x) + (m.E[2][1] * v.y) + (m.E[2][2] * v.z) + (m.E[2][3] * v.w),
        .w = (m.E[3][0] * v.x) + (m.E[3][1] * v.y) + (m.E[3][2] * v.z) + (m.E[3][3] * v.w),
    };
    return r;
}


v3 glmth_v3_cross(v3 vec1, v3 vec2)
{
    v3 r = {
        .x = (vec1.y * vec2.z) - (vec1.z * vec2.y),
        .y = (vec1.z * vec2.x) - (vec1.x * vec2.z),
        .z = (vec1.x * vec2.y) - (vec1.y * vec2.x),
    };
    return r;
}


v3 glmth_v3_init(f32 x, f32 y, f32 z)
{
    v3 v = { .x = x, .y = y, .z = z };
    return v;
}

v3 glmth_v3_init_f(f32 f)
{
    return glmth_v3_init(f, f, f);
}

f32 glmth_v3_length(v3 v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

v3 glmth_v3f_m(v3 v, f32 s)
{
    v3 r = { .x = v.x * s, .y = v.y * s, .z = v.z * s };
    return r;
}

v3 glmth_v3_negate(v3 v)
{
    v3 r = { .x = -v.x, .y = -v.y, .z = -v.z };
    return r;
}


v3 glmth_v3_normalize(v3 v)
{
    f32 l = glmth_v3_length(v);
    v3 r = glmth_v3_init(v.x / l, v.y / l, v.z / l);
    return r;
}


void glmth_v3_print(v3 v)
{
    printf("( %f, %f, %f )\n", v.x, v.y, v.z);
}


v3 glmth_v3_a(v3 vec1, v3 vec2)
{
    v3 r = {
        .x = vec1.x + vec2.x,
        .y = vec1.y + vec2.y,
        .z = vec1.z + vec2.z
    };
    return r;
}


v3 glmth_v3_s(v3 vec1, v3 vec2)
{
    return glmth_v3_a(vec1, glmth_v3_negate(vec2));
}


void glmth_v4_print(v4 v)
{
    printf("( %f, %f, %f, %f )\n", v.x, v.y, v.z, v.w);
}


bool glmth_v4v4_eq(v4 vec1, v4 vec2)
{
    for (u8 i = 0; i < 4; ++i)
    {
        if (vec1.E[i] != vec2.E[i])
        {
            return false;
        }
    }
    return true;
}


void glmth_clampf(float *f, float min, float max)
{
    if (*f < min)
    {
        *f = min;
    }
    else if (*f > max)
    {
        *f = max;
    }
}


f32 glmth_deg(f32 rad)
{
    return rad * (180.0f / M_PI);
}


float glmth_lerpf(float f, float min, float max)
{
    assert(f >= 0.0f && f <= 1.0f);
    return (1.0f - f) * min + f * max;
}


f32 glmth_rad(f32 deg)
{
    return deg * (M_PI / 180.0f);
}


m4 glmth_rotate_x(m4 m, f32 rad)
{
    f32 c = cosf(rad);
    f32 s = sinf(rad);

    m4 r = glmth_m4_init_id();

    r.E[1][1] = c;
    r.E[1][2] = -s;

    r.E[2][1] = s;
    r.E[2][2] = c;

    return glmth_m4m4_m(m, r);
}


m4 glmth_rotate_y(m4 m, f32 rad)
{
    f32 c = cosf(rad);
    f32 s = sinf(rad);

    m4 r = glmth_m4_init_id();

    r.E[0][0] = c;
    r.E[0][2] = s;

    r.E[2][0] = -s;
    r.E[2][2] = c;

    return glmth_m4m4_m(m, r);
}


m4 glmth_rotate_z(m4 m, f32 rad)
{
    f32 c = cosf(rad);
    f32 s = sinf(rad);

    m4 r = glmth_m4_init_id();

    r.E[0][0] = c;
    r.E[0][1] = -s;

    r.E[1][0] = s;
    r.E[1][1] = c;

    return glmth_m4m4_m(m, r);
}


m4 glmth_rotate(m4 m, f32 rad, v3 axis)
{
    axis = glmth_v3_normalize(axis);

    f32 c = cosf(rad);
    f32 s = sinf(rad);

    m4 r = glmth_m4_init_id();

    r.E[0][0] = c + (powf(axis.x, 2.0f) * (1 - c));
    r.E[0][1] = (axis.x * axis.y * (1 - c)) - (axis.z * s);
    r.E[0][2] = (axis.x * axis.z * (1 - c)) + (axis.y * s);

    r.E[1][0] = (axis.y * axis.x * (1 - c)) + (axis.z * s);
    r.E[1][1] = c + (powf(axis.y, 2.0f) * (1 - c));
    r.E[1][2] = (axis.y * axis.z * (1 - c)) - (axis.x * s);

    r.E[2][0] = (axis.z * axis.x * (1 - c)) - (axis.y * s);
    r.E[2][1] = (axis.z * axis.y * (1 - c)) + (axis.x * s);
    r.E[2][2] = c + (powf(axis.z, 2.0f) * (1 - c));

    return glmth_m4m4_m(m, r);
}


m4 glmth_scale(m4 m, v3 v)
{
    m4 r = glmth_m4_init_id();
    r.E[0][0] = v.x;
    r.E[1][1] = v.y;
    r.E[2][2] = v.z;
    return glmth_m4m4_m(m, r);
}


m4 glmth_translate(m4 m, v3 v)
{
    m4 r = glmth_m4_init_id();
    r.E[0][3] = v.x;
    r.E[1][3] = v.y;
    r.E[2][3] = v.z;
    return glmth_m4m4_m(m, r);
}


m4 glmth_projection_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    assert(left != right);
    assert(bottom != top);
    assert(near != far);

    m4 r = glmth_m4_init_id();

    r.E[0][0] = 2.0f / (right - left);
    r.E[0][1] = 0.0f;
    r.E[0][2] = 0.0f;
    r.E[0][3] = -(right + left) / (right - left);

    r.E[1][0] = 0.0f;
    r.E[1][1] = 2.0f / (top - bottom);
    r.E[1][2] = 0.0f;
    r.E[1][3] = -(top + bottom) / (top - bottom);

    r.E[2][0] = 0.0f;
    r.E[2][1] = 0.0f;
    r.E[2][2] = -2.0f / (far - near);
    r.E[2][3] = -(far + near) / (far - near);

    r.E[3][0] = 0.0f;
    r.E[3][1] = 0.0f;
    r.E[3][2] = 0.0f;
    r.E[3][3] = 1.0f;

    return r;
}


m4 glmth_projection_perspective(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    assert(left != right);
    assert(bottom != top);
    assert(near != far);

    m4 r = glmth_m4_init_id();

    r.E[0][0] = (2.0f * near) / (right - left);
    r.E[0][1] = 0.0f;
    r.E[0][2] = (right + left) / (right - left);
    r.E[0][3] = 0.0f;

    r.E[1][0] = 0.0f;
    r.E[1][1] = (2.0f * near) / (top - bottom);
    r.E[1][2] = (top + bottom) / (top - bottom);
    r.E[1][3] = 0.0f;

    r.E[2][0] = 0.0f;
    r.E[2][1] = 0.0f;
    r.E[2][2] = -(far + near) / (far - near);
    r.E[2][3] = (-2.0f * far * near) / (far - near);

    r.E[3][0] = 0.0f;
    r.E[3][1] = 0.0f;
    r.E[3][2] = -1.0f;
    r.E[3][3] = 0.0f;

    return r;
}


m4 glmth_projection_perspective_fov(f32 fovy, f32 aspect, f32 near, f32 far)
{
    f32 half_height = tanf(fovy / 2.0f) * near;
    f32 half_width = half_height * aspect;
    f32 left = -half_width;
    f32 right = half_width;
    f32 bottom = -half_height;
    f32 top = half_height;

    return glmth_projection_perspective(left, right, bottom, top, near, far);
}


m4 glmth_camera_look_at(v3 camera_pos, v3 camera_target, v3 up)
{
    v3 camera_direction = glmth_v3_normalize(glmth_v3_s(camera_pos, camera_target));
    v3 camera_right = glmth_v3_normalize(glmth_v3_cross(up, camera_direction));
    v3 camera_up = glmth_v3_cross(camera_direction, camera_right);

    m4 look = glmth_m4_init_id();
    look.E[0][0] = camera_right.x;
    look.E[0][1] = camera_right.y;
    look.E[0][2] = camera_right.z;

    look.E[1][0] = camera_up.x;
    look.E[1][1] = camera_up.y;
    look.E[1][2] = camera_up.z;

    look.E[2][0] = camera_direction.x;
    look.E[2][1] = camera_direction.y;
    look.E[2][2] = camera_direction.z;

    return glmth_m4m4_m(look, glmth_translate(glmth_m4_init_id(), glmth_v3_negate(camera_pos)));
}
