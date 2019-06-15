#pragma once

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef GAME_WEBGL
#include "webgl.h"
#else
#include "glad/glad.h"
#endif

#include "glmth.h"
#include "shader.h"
#include "platform.h"

void game_init(struct GameState *game_state, u32 screen_width, u32 screen_height);
void game_cleanup(struct GameState *game_state);
