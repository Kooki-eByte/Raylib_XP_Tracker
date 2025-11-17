#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include "../defines.h"
#include "../../include/raylib.h"
#include "../../include/raygui.h"
#include "../utilities/data_persistence.h"
#include "../utilities/settings.h"

// Use data_persistence to get all of the user_save_data and return a saveDataSelector struct for LoadMenuComponent
saveDataSelector GetUserSaveContent();

void LoadMenuComponent(saveDataSelector *sds, xp_window_settings *ws, enum GameState *gs);
