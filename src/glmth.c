#include "glmth.h"

#include <stdio.h>
#include <stdlib.h>

m4 glmth_m4_init_id(void)
{
    m4 m = { 0 };
    m.E[0][0] = 1.0f;
    m.E[1][1] = 1.0f;
    m.E[2][2] = 1.0f;
    m.E[3][3] = 1.0f;
    return m;
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


v3 glmth_v3_init(f32 x, f32 y, f32 z)
{
    v3 v = { .x = x, .y = y, .z = z };
    return v;
}

f32 glmth_v3_length(v3 v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

v3 glmth_v3_normalize(v3 v)
{
    f32 l = glmth_v3_length(v);
    v3 r = glmth_v3_init(v.x / l, v.y / l, v.z / l);
    return r;
}

f32 glmth_rad(f32 deg)
{
    return deg * (M_PI / 180.0f);
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

m4 glmth_translate(m4 m, v3 v)
{
    m4 r = glmth_m4_init_id();
    r.E[0][3] = v.x;
    r.E[1][3] = v.y;
    r.E[2][3] = v.z;
    return glmth_m4m4_m(m, r);
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
