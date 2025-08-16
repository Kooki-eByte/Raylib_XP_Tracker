#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../defines.h"
#include "../../include/raylib.h"

#define SAVE_BIN "./save_data/"

typedef struct user_project_data {
  char project_name[255];
  b32 user_level;
  f32 current_exp;
  f32 max_exp;
} user_project_data;

typedef struct saveDataSelector {
  char *user_save_data[3];
  b32 selected_index;
} saveDataSelector;

bool save_data(user_project_data *user_data);

void load_data(user_project_data *user_data);

u8 delete_data(char *fileName);