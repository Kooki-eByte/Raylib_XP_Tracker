#include "LoadMenuComponent.h"

saveDataSelector GetUserSaveContent() {
  DIR *dir;
  struct dirent *entry;
  
  saveDataSelector saveData = {0};

  dir = opendir(SAVE_BIN);
  if (dir == NULL) {
    fprintf(stderr, "Unable to open directory");
  }

  b32 ind = 0;

  while ((entry = readdir(dir)) != NULL) {
    if (strlen(entry->d_name) <= 1 || strstr(entry->d_name, "..\0")) {
      printf("not reading the . and .. of the directories\n");
      continue;
    }

    printf("Filename: %s\n", entry->d_name);
    saveData.user_save_data[ind++] = entry->d_name; 
  }

  return saveData;
}

void LoadMenuComponent(saveDataSelector *sds, xp_window_settings *ws) {
  BeginDrawing();
    // if (GuiButton((Rectangle){ center_screen_pos_x - (120 / 2), center_screen_pos_y - (30 / 2), 120, 30 }, "LOAD FILE")) {
    //   is_load_file_menu = false;
    // }

    // Create Save files available to click as well as a delete button
    for (b32 saves = 0; saves < 3; saves++) {
      // If less than 3 save files are found then skip step
      if (sds->user_save_data[saves] == NULL) continue;
      char *save_title = sds->user_save_data[saves];

      DrawText(save_title, (ws->width / 2) - 75, (ws->height / 2.25) + (20 * saves), 16, BEIGE);
      
      /* FIXME: When clicking delete the screen will still work but
      the delete button will flicker blue constantly and the
      save data content will not update for some reason */
      if (GuiButton((Rectangle){ (ws->width / 2) + 100, (ws->height / 2.25) + (20 * saves), 100, 20 }, "Delete Save")) {
        if (delete_data(save_title)) {
          *sds = GetUserSaveContent();
        }
      }
    }

    if (GuiButton((Rectangle){ (ws->width / 2), (ws->height / 2.25) + 200, 100, 20 }, "Load Save")) {
      sds->selected_index = 1;
    } 

  EndDrawing(); 
}