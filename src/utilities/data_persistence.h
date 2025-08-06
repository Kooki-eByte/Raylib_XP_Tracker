#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../defines.h"
#include "../../include/raylib.h"

#define SAVE_BIN "./save_bin/"

typedef struct UserProjectData
{
  char project_name[255];
  b32 user_level;
  f32 current_exp;
  f32 max_exp;
} UserProjectData;

bool save_data(const UserProjectData *user_data);

void load_data(UserProjectData *user_data);