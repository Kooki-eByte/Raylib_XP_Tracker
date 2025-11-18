#pragma once
#include "../defines.h"

typedef struct xp_window_settings {
  i32 width;
  i32 height;
  char *title;
  i32 fps;
} xp_window_settings;

enum GameState {
  PROJ_MENU,      // Start of game loop, selecting a project
  NEW_PROJ,       // When user select making a new project
  PROJ_SELECTED   // When user has either selected a new project or created new one
};