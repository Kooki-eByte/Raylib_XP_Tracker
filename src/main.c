#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// From inlcude dir
#include "../include/raylib.h"

// From src dir
#include "defines.h"
#include "components/InputFieldComponent.h"
#include "utilities/data_persistence.h"

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

const i32 FPS = 60; 

typedef struct xp_window_settings {
  i32 width;
  i32 height;
  char *title;
} xp_window_settings;

int get_window_center_x(xp_window_settings *ws) {
  return (ws->height / 2);
} 

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

bool gain_exp_hour(b32 hours_spent,f32 *exp, f32 *current_max_exp) {
  if (hours_spent <= 0) return false;
  
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
  if (min_spent <= 0) return false;
  
  // Formula for exp gained
  f32 exp_gained = min_spent * 2 + 5;
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

void handle_level_up(b32 *user_lvl) {
  *user_lvl += 1;
}

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
  xp_window_settings window_settings;
  window_settings.width = 720;
  window_settings.height = 480;
  window_settings.title = "XP Tracker";

  // Initial user data
  user_project_data user_data = {
    .current_exp = 0.0f,
    .max_exp = 100.0f,
    .user_level = 1,
    .project_name = "c_programming"
  };

  load_data(user_data);

  InitWindow(window_settings.width, window_settings.height, window_settings.title);
  SetTargetFPS(FPS);

  // Helper variables //
  int center_screen_pos_x = (window_settings.width / 2);
  int center_screen_pos_y = (window_settings.height / 2);

  // TODO: Uncomment after plugging in dark styling into proj.
  // GuiLoadStyleDark();

  bool showMessageBox = false;

  
  // Input field vars //
  NumberInputField hoursInput = {
    .bounds = (Rectangle) {center_screen_pos_x - 150, (center_screen_pos_y / 2) + 20, 100 ,50},
    .isFocused = false,
    .length = 1,
    .value = {'0', '\0'}
  };
  
  NumberInputField minutesInput = {
    .bounds = (Rectangle) {center_screen_pos_x + 50, (center_screen_pos_y / 2) + 20, 100 ,50},
    .isFocused = false,
    .length = 1,
    .value = {'0' ,'\0'},
  };
  
  cursor_settings cursor_setting = {
    .cursorTimer = 0.0f,
    .cursorVisible = false,
    .cursor_ON_time = 1.0f,
    .cursor_OFF_time = 0.5f
  };
  
  // User data
  // TODO: Get this information from save file data
  f32 current_exp = 0.0f;
  f32 current_max_exp = 100.0f;
  b32 current_level = 1;

  f32 *current_exp_p = &current_exp;
  f32 *current_max_exp_p = &current_max_exp;
  b32 *current_level_p = &current_level;

  

  // Main loop
  while (!WindowShouldClose()) {
    update_number_input_field(&hoursInput, &cursor_setting);
    update_number_input_field(&minutesInput, &cursor_setting);
    
    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    
    DrawText("LVL:", 10, 10, 20, DARKGRAY);
    DrawText(int_to_string(current_level), 60, 10, 20, BLACK);

    GuiProgressBar(
      (Rectangle){ get_window_center_x(&window_settings), 50, window_settings.width / 4, 25 },
        "EXP BAR:",
        get_exp_percentage(current_max_exp, current_exp_p),
        current_exp_p,
        0.0f,
        current_max_exp
      );
      
      if (GuiButton((Rectangle){ center_screen_pos_x - (120 / 2), center_screen_pos_y - (30 / 2), 120, 30 }, "SUMBIT TIME")) {
        if (hoursInput.length > 0) {
          if (gain_exp_hour(atoi(hoursInput.value), current_exp_p, current_max_exp_p)) {
            handle_level_up(current_level_p);
            showMessageBox = true;
          }
        }
        if (minutesInput.length > 0) {
          if (gain_exp_min(atoi(minutesInput.value), current_exp_p, current_max_exp_p)) {
            handle_level_up(current_level_p);
            showMessageBox = true;
          }
        }

        if (save_data(user_data)) {
          TraceLog(LOG_INFO, "File [%s.dat] Saved successfully!", user_data.project_name);
        } else {
          TraceLog(LOG_WARNING, "File [%s.dat] was not saved successfully! T_T", user_data.project_name);
        }
      }

      if (showMessageBox) {
      int result = GuiMessageBox((Rectangle){ (window_settings.width / 2) - 125, (window_settings.height / 2) - 50, 250, 100 }, "#191#LEVEL UP!", "You Leveled Up! Congrats!", "YIPPEE!");

        if (result >= 0) showMessageBox = false;
      }

      DrawText("Hours:", center_screen_pos_x - 150, (center_screen_pos_y / 2), 20, DARKGRAY);
      DrawText("Minutes:", center_screen_pos_x + 50, (center_screen_pos_y / 2), 20, DARKGRAY);

      draw_number_input_field(&hoursInput, &cursor_setting);
      draw_number_input_field(&minutesInput, &cursor_setting);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
