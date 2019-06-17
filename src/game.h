#pragma once

// TODO: Clean up
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#include <stddef.h>
#include <stdint.h>
#define static_assert _Static_assert

static_assert(__has_builtin(__builtin_cosf), "cosf");
static_assert(__has_builtin(__builtin_sinf), "sinf");
static_assert(__has_builtin(__builtin_tanf), "tanf");
static_assert(__has_builtin(__builtin_sqrtf), "sqrtf");
static_assert(__has_builtin(__builtin_powf), "powf");
#define cosf __builtin_cosf
#define sinf __builtin_sinf
#define tanf __builtin_tanf
#define sqrtf __builtin_sqrtf
#define powf __builtin_powf

#define NULL ((void*)0)

// TODO: fix this
#define assert(x) (void)0

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
