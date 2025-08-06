#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// From inlcude dir
#include "../include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

// From src dir
#include "defines.h"
#include "components/InputFieldComponent.h"
#include "utilities/data_persistence.h"
#include "utilities/exp_handler.h"

const i32 FPS = 60;

typedef struct xp_window_settings
{
  i32 width;
  i32 height;
  char *title;
} xp_window_settings;

int get_window_center_x(xp_window_settings *ws)
{
  return (ws->height / 2);
}

char *int_to_string(b32 num)
{
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

int main(void)
{
  xp_window_settings window_settings;
  window_settings.width = 720;
  window_settings.height = 480;
  window_settings.title = "XP Tracker";

  // Initial user data
  UserProjectData user_data = {
      .current_exp = 0.0f,
      .max_exp = 100.0f,
      .user_level = 1,
      .project_name = "c_programming"};

  load_data(&user_data);

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
      .bounds = (Rectangle){center_screen_pos_x - 150, (center_screen_pos_y / 2) + 20, 100, 50},
      .isFocused = false,
      .length = 1,
      .value = {'0', '\0'}};

  NumberInputField minutesInput = {
      .bounds = (Rectangle){center_screen_pos_x + 50, (center_screen_pos_y / 2) + 20, 100, 50},
      .isFocused = false,
      .length = 1,
      .value = {'0', '\0'},
  };

  cursor_settings cursor_setting = {
      .cursorTimer = 0.0f,
      .cursorVisible = false,
      .cursor_ON_time = 1.0f,
      .cursor_OFF_time = 0.5f};

  // Main loop
  while (!WindowShouldClose())
  {
    update_number_input_field(&hoursInput, &cursor_setting);
    update_number_input_field(&minutesInput, &cursor_setting);

    BeginDrawing();
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    DrawText("LVL:", 10, 10, 20, DARKGRAY);
    DrawText(int_to_string(user_data.user_level), 60, 10, 20, BLACK);

    GuiProgressBar(
        (Rectangle){get_window_center_x(&window_settings), 50, window_settings.width / 4, 25},
        "EXP BAR:",
        getExpPercentage(user_data.max_exp, user_data.current_exp),
        &user_data.current_exp,
        0.0f,
        user_data.max_exp);

    // Submit button handling when clicked
    if (GuiButton((Rectangle){center_screen_pos_x - (120 / 2), center_screen_pos_y - (30 / 2), 120, 30}, "SUMBIT TIME"))
    {
      if (hoursInput.length > 0)
      {
        if (gainHourExp(atoi(hoursInput.value), &user_data.current_exp, &user_data.max_exp))
        {
          handleLevelUp(&user_data);
          showMessageBox = true;
        }
      }
      if (minutesInput.length > 0)
      {
        if (gainMinuteExp(atoi(minutesInput.value), &user_data.current_exp, &user_data.max_exp))
        {
          handleLevelUp(&user_data);
          showMessageBox = true;
        }
      }

      if (save_data(&user_data))
      {
        TraceLog(LOG_INFO, "File [%s] Saved successfully!", user_data.project_name);
      }
      else
      {
        TraceLog(LOG_WARNING, "File [%s] was not saved successfully! T_T", user_data.project_name);
      }
    }

    if (showMessageBox)
    {
      int result = GuiMessageBox((Rectangle){(window_settings.width / 2) - 125, (window_settings.height / 2) - 50, 250, 100}, "#191#LEVEL UP!", "You Leveled Up! Congrats!", "YIPPEE!");

      if (result >= 0)
        showMessageBox = false;
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
