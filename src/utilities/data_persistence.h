#pragma once
#include <stdlib.h>
#include <string.h>

#include "../defines.h"
#include "../../include/raylib.h"

typedef struct user_project_data {
  char *project_name;
  b32 user_level;
  f32 current_exp;
  f32 max_exp;
} user_project_data;

user_project_data save_data(user_project_data *user_data);

user_project_data load_data(char *project);