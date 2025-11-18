// TODO: Fix this crap lol I cn deff make this way better with a fresh sweep on
// it all.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// From inlcude dir
#include "../include/raylib.h"

// From src dir
#include "defines.h"

// Components
#include "components/InputFieldComponent.h"
#include "components/LoadMenuComponent.h"

// Utilities
#include "utilities/data_persistence.h"
#include "utilities/settings.h"

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

int get_window_center_x(xp_window_settings *ws) { return (ws->height / 2); }
int get_window_center_y(xp_window_settings *ws) { return (ws->width / 2); }

char *get_exp_percentage(f32 max_exp, f32 *current_exp) {
  f32 exp_percent_remaining = *current_exp / max_exp * 100;

  char buffer[32] = "EXP REMAINING: ";
  u32 offset = strlen(buffer);

  i32 length = snprintf(buffer + offset, 32, "%.2f%%", exp_percent_remaining);

  offset += length;

  char *exp_remaining;
  exp_remaining = (char *)malloc(sizeof(buffer));
  memcpy(exp_remaining, buffer, sizeof(buffer));
  exp_remaining[sizeof(buffer)] = '\0';

  return exp_remaining;
}

bool gain_exp_hour(b32 hours_spent, f32 *exp, f32 *current_max_exp) {
  if (hours_spent <= 0)
    return false;

  // Formula for exp gained
  f32 exp_gained = hours_spent * 10 + 15;
  //
  f32 temp_exp_hold = *exp + exp_gained;

  if (temp_exp_hold >= *current_max_exp) {
    *exp = fmodf(temp_exp_hold, *current_max_exp);
    *current_max_exp *= 2;
    return true;
  }

  *exp = temp_exp_hold;
  return false;
}

bool gain_exp_min(b32 min_spent, f32 *exp, f32 *current_max_exp) {
  if (min_spent <= 0)
    return false;

  // Formula for exp gained
  f32 exp_gained = min_spent * 2 + 5;
  f32 temp_exp_hold = *exp + exp_gained;

  if (temp_exp_hold >= *current_max_exp) {
    *exp = fmodf(temp_exp_hold, *current_max_exp);
    *current_max_exp *= 2;
    return true;
  }

  *exp = temp_exp_hold;
  return false;
}

void handle_level_up(b32 *user_lvl) { *user_lvl += 1; }

char *int_to_string(b32 num) {
  char str_buffer[5] = "";
  u32 offset = strlen(str_buffer);

  i32 length = snprintf(str_buffer + offset, 5, "%d", num);

  offset += length;

  char *intToString;
  intToString = (char *)malloc(sizeof(str_buffer));
  memcpy(intToString, str_buffer, sizeof(str_buffer));
  intToString[sizeof(str_buffer)] = '\0';

  return intToString;
}

int main(void) {
  enum GameState game_state = PROJ_MENU;

  xp_window_settings window_settings = {
      .width = 720, .height = 480, .title = "XP Tracker", .fps = 60};

  // Initial user data
  user_project_data user_data = {.current_exp = 0.0f,
                                 .max_exp = 100.0f,
                                 .user_level = 1,
                                 .project_name = ""};

  InitWindow(window_settings.width, window_settings.height,
             window_settings.title);
  SetTargetFPS(window_settings.fps);

  // Helper variables //
  int center_screen_pos_x = (window_settings.width / 2);
  int center_screen_pos_y = (window_settings.height / 2);

  // TODO: Uncomment after plugging in dark styling into proj.
  // GuiLoadStyleDark();

  bool showMessageBox = false;

  // Input field vars //
  NumberInputField hoursInput = {
      .bounds = (Rectangle){center_screen_pos_x - 150,
                            (center_screen_pos_y / 2) + 20, 100, 50},
      .isFocused = false,
      .length = 1,
      .value = {'0', '\0'}};

  NumberInputField minutesInput = {
      .bounds = (Rectangle){center_screen_pos_x + 50,
                            (center_screen_pos_y / 2) + 20, 100, 50},
      .isFocused = false,
      .length = 1,
      .value = {'0', '\0'},
  };

  TextInputField createNewProjectField = {
    .bounds = (Rectangle){center_screen_pos_x - 150,
                            (center_screen_pos_y / 2) + 75, 300, 48},
    .isFocused = false,
    .length = 11,
    .value = {'p', 'l', 'a', 'c', 'e', 'h', 'o', 'l', 'd', 'e', 'r', '\0'}
  };
  // 

  cursor_settings cursor_setting = {.cursorTimer = 0.0f,
                                    .cursorVisible = false,
                                    .cursor_ON_time = 1.0f,
                                    .cursor_OFF_time = 0.5f};

  saveDataSelector save_data_content = GetUserSaveContent();

  // Game loop
  while (!WindowShouldClose()) {
    // Run load menu
    if (game_state == PROJ_MENU) {
      // run load menu component
      if (save_data_content.selected_index == 0) {
        LoadMenuComponent(&save_data_content, &window_settings, &game_state);
      }
      if (save_data_content.selected_index >= 1 &&
          save_data_content.selected_index <= 3) {
        char *project =
            save_data_content
                .user_save_data[(save_data_content.selected_index - 1)];
        strcpy_s(user_data.project_name, sizeof(user_data.project_name),
                 project);
        load_data(&user_data);
        game_state = PROJ_SELECTED;
      }
    }
    
    if (game_state == NEW_PROJ) {
      // Make a new save
      update_text_input_field(&createNewProjectField, &cursor_setting);

      BeginDrawing();
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      DrawText("Please name your new project to track:", get_window_center_y(&window_settings) - 150,
                get_window_center_x(&window_settings) - 120, 20, DARKGRAY);

      draw_text_input_field(&createNewProjectField, &cursor_setting);
      
      if (GuiButton((Rectangle){ 
        get_window_center_y(&window_settings) - 50,
        (get_window_center_x(&window_settings) / 2.25) + 200,
        150,
        30 
      }, "Make Project")) {
        if (strlen(createNewProjectField.value) > 2) {
          strcpy_s(user_data.project_name, sizeof(user_data.project_name), createNewProjectField.value);
          
          game_state = PROJ_SELECTED;
        } else {
          fprintf(stderr, "[ERR]: failed to make new project!");
          return false;
        }
      }

      EndDrawing();
    } 
      
    if (game_state == PROJ_SELECTED) {
      // Main loop
      update_number_input_field(&hoursInput, &cursor_setting);
      update_number_input_field(&minutesInput, &cursor_setting);

      BeginDrawing();
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      DrawText("LVL:", 10, 10, 20, DARKGRAY);
      DrawText(int_to_string(user_data.user_level), 60, 10, 20, BLACK);

      GuiProgressBar(
          (Rectangle){get_window_center_x(&window_settings), 50,
                      window_settings.width / 4, 25},
          "EXP BAR:",
          get_exp_percentage(user_data.max_exp, &user_data.current_exp),
          &user_data.current_exp, 0.0f, user_data.max_exp);

      if (GuiButton((Rectangle){center_screen_pos_x - (120 / 2),
                                center_screen_pos_y - (30 / 2), 120, 30},
                    "SUMBIT TIME")) {
        if (hoursInput.length > 0) {
          if (gain_exp_hour(atoi(hoursInput.value), &user_data.current_exp,
                            &user_data.max_exp)) {
            handle_level_up(&user_data.user_level);
            showMessageBox = true;
          }
        }
        if (minutesInput.length > 0) {
          if (gain_exp_min(atoi(minutesInput.value), &user_data.current_exp,
                            &user_data.max_exp)) {
            handle_level_up(&user_data.user_level);
            showMessageBox = true;
          }
        }

        if (save_data(&user_data)) {
          TraceLog(LOG_INFO, "File [%s] Saved successfully!",
                    user_data.project_name);
        } else {
          TraceLog(LOG_WARNING, "File [%s] was not saved successfully! T_T",
                    user_data.project_name);
        }
      }

      if (showMessageBox) {
        int result = GuiMessageBox(
            (Rectangle){(window_settings.width / 2) - 125,
                        (window_settings.height / 2) - 50, 250, 100},
            "#191#LEVEL UP!", "You Leveled Up! Congrats!", "YIPPEE!");

        if (result >= 0)
          showMessageBox = false;
      }

      DrawText("Hours:", center_screen_pos_x - 150, (center_screen_pos_y / 2),
                20, DARKGRAY);
      DrawText("Minutes:", center_screen_pos_x + 50, (center_screen_pos_y / 2),
                20, DARKGRAY);

      draw_number_input_field(&hoursInput, &cursor_setting);
      draw_number_input_field(&minutesInput, &cursor_setting);
      EndDrawing();
    }
  }

  for (int savedata_index = 0; savedata_index < 3; savedata_index++) {
    free(save_data_content.user_save_data[savedata_index]);
  }

  CloseWindow();

  return 0;
}
