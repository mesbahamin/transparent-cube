#pragma once

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;
typedef float              f32;
typedef double             f64;

#define GLMTH_PI        3.14159265358979323846f
#define GLMTH_M_2_PI    6.28318530717958647693f
#define GLMTH_D_4_PI    1.27323954473516268615f
#define GLMTH_D_PI_2    1.57079632679489661923f
#define GLMTH_D_4_SQ_PI 0.40528473456935108578f

static inline f32 glmth_floorf(f32 x)
{
    f32 result = (f32)((i32)x - (x < 0.0f));
    return result;
}

// These Sine and Cosine approximations use a parabola adjusted to minimize
// error. This lovely approximation was derived by "Nick" on the devmaster.net
// forums:
//
// https://web.archive.org/web/20080228213915/http://www.devmaster.net/forums/showthread.php?t=5784
static inline f32 glmth_sinf(f32 x)
{

    // wrap to range [0, 2PI]
    f32 full_circles = glmth_floorf(x / GLMTH_M_2_PI);
    f32 full_circle_radians = full_circles * GLMTH_M_2_PI;
    x = x - full_circle_radians;

    // TODO: remove branches
    // wrap to range [-PI, PI]
    if(x < -GLMTH_PI)
    {
        x += GLMTH_M_2_PI;
    }
    else if(x > GLMTH_PI)
    {
        x -= GLMTH_M_2_PI;
    }

    // fit parabola to sine
    f32 f = 0.0f;
    f32 g = 0.0f;
    if(x < 0.0f)
    {
        f = (GLMTH_D_4_PI * x) + (GLMTH_D_4_SQ_PI * x * x);
        g = f * -f;

    }
    else
    {
        f = (GLMTH_D_4_PI * x) - (GLMTH_D_4_SQ_PI * x * x);
        g = f * f;
    }

    f32 h = g - f;
    f32 i = h * 0.225f;
    f = i + f;

    return f;
}

static inline f32 glmth_cosf(f32 x)
{
    // wrap to range [0, 2PI]
    f32 full_circles = glmth_floorf(x / GLMTH_M_2_PI);
    f32 full_circle_radians = full_circles * GLMTH_M_2_PI;
    x = x - full_circle_radians;

    // TODO: remove branches
    // wrap to range [-PI, PI]
    if(x < -GLMTH_PI)
    {
        x += GLMTH_M_2_PI;
    }
    else if(x > GLMTH_PI)
    {
        x -= GLMTH_M_2_PI;
    }

    x += GLMTH_D_PI_2;
    if(x > GLMTH_PI)
    {
        x -= GLMTH_M_2_PI;
    }

    // fit parabola to cosine
    f32 f = 0.0f;
    f32 g = 0.0f;
    if(x < 0.0f)
    {
        f = (GLMTH_D_4_PI * x) + (GLMTH_D_4_SQ_PI * x * x);
        g = f * -f;

    }
    else
    {
        f = (GLMTH_D_4_PI * x) - (GLMTH_D_4_SQ_PI * x * x);
        g = f * f;
    }

    f32 h = g - f;
    f32 i = h * 0.225f;
    f = i + f;
    return f;
}

static inline f32 glmth_tanf(f32 x)
{
    // TODO: make a proper approximation
    return glmth_sinf(x)/glmth_cosf(x);
}

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

static inline m4 glmth_m4_init_id(void)
{
    m4 m = { 0 };
    m.E[0][0] = 1.0f;
    m.E[1][1] = 1.0f;
    m.E[2][2] = 1.0f;
    m.E[3][3] = 1.0f;
    return m;
}

static inline void glmth_m4_valueptr(m4 m, f32* out_valueptr)
{
    for (u8 v = 0; v < 16; ++v)
    {
        u8 row = v / 4;
        u8 col = v % 4;
        out_valueptr[v] = m.E[row][col];
    }
}

static inline m4 glmth_m4m4_m(m4 mat1, m4 mat2)
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

static inline v3 glmth_v3_init(f32 x, f32 y, f32 z)
{
    v3 v = { .x = x, .y = y, .z = z };
    return v;
}

static inline f32 glmth_rad(f32 deg)
{
    return deg * (M_PI / 180.0f);
}

static inline m4 glmth_rotate(m4 m, f32 rad, v3 axis)
{
    f32 c = glmth_cosf(rad);
    f32 s = glmth_sinf(rad);

    m4 r = glmth_m4_init_id();

    r.E[0][0] = c + ((axis.x * axis.x) * (1 - c));
    r.E[0][1] = (axis.x * axis.y * (1 - c)) - (axis.z * s);
    r.E[0][2] = (axis.x * axis.z * (1 - c)) + (axis.y * s);

    r.E[1][0] = (axis.y * axis.x * (1 - c)) + (axis.z * s);
    r.E[1][1] = c + ((axis.y * axis.y) * (1 - c));
    r.E[1][2] = (axis.y * axis.z * (1 - c)) - (axis.x * s);

    r.E[2][0] = (axis.z * axis.x * (1 - c)) - (axis.y * s);
    r.E[2][1] = (axis.z * axis.y * (1 - c)) + (axis.x * s);
    r.E[2][2] = c + ((axis.z * axis.z) * (1 - c));

    return glmth_m4m4_m(m, r);
}

static inline m4 glmth_translate(m4 m, v3 v)
{
    m4 r = glmth_m4_init_id();
    r.E[0][3] = v.x;
    r.E[1][3] = v.y;
    r.E[2][3] = v.z;
    return glmth_m4m4_m(m, r);
}

static inline m4 glmth_projection_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    // TODO: This assert fails when you minimise the window in Windows.
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

static inline m4 glmth_projection_perspective(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
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

static inline m4 glmth_projection_perspective_fov(f32 fovy, f32 aspect, f32 near, f32 far)
{
    f32 half_height = glmth_tanf(fovy / 2.0f) * near;
    f32 half_width = half_height * aspect;
    f32 left = -half_width;
    f32 right = half_width;
    f32 bottom = -half_height;
    f32 top = half_height;

    return glmth_projection_perspective(left, right, bottom, top, near, far);
}
