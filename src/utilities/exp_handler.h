#pragma once

#include <stdlib.h>
#include <math.h>

#include "../defines.h"
#include "../../include/raylib.h"
#include "data_persistence.h"

// static f32 updateMaxExp(b32 level);

char *getExpPercentage(f32 max_exp, f32 current_exp);

bool gainHourExp(b32 hours_spent, f32 *exp, f32 *current_max_exp);
bool gainMinuteExp(b32 minutes_spent, f32 *exp, f32 *current_max_exp);

void handleLevelUp(UserProjectData *user_lvl);