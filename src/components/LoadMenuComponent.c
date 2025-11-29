#include "LoadMenuComponent.h"

#define MAX_SAVE_CHAR_LIMIT 256

saveDataSelector GetUserSaveContent() {
  DIR *dir;
  struct dirent *entry;

  saveDataSelector saveData = {0};

  // NOTE:(Cristian) Need to allocate for each string in the array.
  // So we can put these in actual locations in memory.
  saveData.user_save_data[0] = malloc(sizeof(char) * MAX_SAVE_CHAR_LIMIT);
  strcpy(saveData.user_save_data[0], "\0");
  saveData.user_save_data[1] = malloc(sizeof(char) * MAX_SAVE_CHAR_LIMIT);
  strcpy(saveData.user_save_data[1], "\0");
  saveData.user_save_data[2] = malloc(sizeof(char) * MAX_SAVE_CHAR_LIMIT);
  strcpy(saveData.user_save_data[2], "\0");

  char *save_file_dir = get_save_dir("xp_tracker/save_data"); 

  dir = opendir(save_file_dir);
  if (dir == NULL) {
    fprintf(stderr, "Unable to open directory\n");
  }

  b32 ind = 0;

  while ((entry = readdir(dir)) != NULL) {
    if (strlen(entry->d_name) <= 1 || strstr(entry->d_name, "..\0")) {
      continue;
    }

    strcpy(saveData.user_save_data[ind], entry->d_name);
    ind++;
    saveData.existing_projects_count = ind;
  }

  free(save_file_dir);
  return saveData;
}

void LoadMenuComponent(saveDataSelector *sds, xp_window_settings *ws, enum GameState *gs) {
  u8 enable_create_button = 0;
  
  BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // Create Save files available to click as well as a delete button
    for (b32 saves = 0; saves < 3; saves++) {
      // If less than 3 save files are found then skip step
      if (sds->user_save_data[saves] == NULL || strlen(sds->user_save_data[saves]) == 0) {
        enable_create_button = 1;
        continue;
      }

      char save_title[MAX_SAVE_CHAR_LIMIT];
      strncpy(save_title, sds->user_save_data[saves], MAX_SAVE_CHAR_LIMIT - 1);
      save_title[MAX_SAVE_CHAR_LIMIT - 1] = '\0';  // Ensure null termination

      DrawText(save_title, (ws->width / 2) - 250, (ws->height / 2.25) + (20 * saves), 20, DARKGRAY);

      // Handle Delete
      if (GuiButton((Rectangle){ (ws->width / 2) + 150, (ws->height / 2.25) + (20 * saves), 100, 20 }, "Delete Save")) {
        if (delete_data(save_title)) {
          *sds = GetUserSaveContent();
        }
      }

      // Handle Loading file
      if (GuiButton((Rectangle){ (ws->width / 2) + 30, (ws->height / 2.25) + (20 * saves), 100, 20 }, "LOAD FILE")) {
        sds->selected_index = saves + 1;
      }
    }

    if (enable_create_button == 1) {
      if (GuiButton((Rectangle){ (ws->width / 2) - 50, (ws->height / 2.25) + 200, 150, 30 }, "Create New Project")) {
        // NOTE: Anything greater than 3 means that we are making a new project save file
        sds->selected_index = 4;
        *gs = NEW_PROJ;
      }
    }

  EndDrawing();
}
