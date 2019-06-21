#pragma once

#include <stddef.h>
#include <stdint.h>
#define static_assert _Static_assert

#define NULL ((void*)0)

// TODO: fix this
#define assert(x) (void)0

inline float sinf(float x)
{
    return x;
}

inline float cosf(float x)
{
    return x;
}

inline float powf(float x, float p)
{
    return x * x;
}

inline float sqrtf(float a)
{
    return a * 0.5f;
}

inline float tanf(float a)
{
    return 0.4142135623730950488f;
}

typedef _Bool bool;
#define true 1
#define false 0

#ifdef GAME_WEBGL
#include "webgl.h"
#else
#include "glad/glad.h"
#endif

#include "glmth.h"
#include "shader.h"
#include "platform.h"

void *memcpy(void *dst, const void *src, size_t n)
{
  u8 *destination = (u8 *)dst;
  u8 *source = (u8 *)src;

  while (n--) {
    *destination = *source;
    destination++;
    source++;
  }

  return dst;
}

// TODO: Consider finding a better way to make this func available
static platform_print_func* print;

void game_init(struct GameState *game_state, u32 screen_width, u32 screen_height);
void game_cleanup(struct GameState *game_state);
