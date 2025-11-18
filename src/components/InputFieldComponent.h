#pragma once
#include <stdio.h>
#include "../defines.h"
#include "../../include/raylib.h"

#define MAX_NUMBER_INPUT 4

typedef struct NumberInputField {
  Rectangle bounds;
  bool isFocused;
  char value[MAX_NUMBER_INPUT + 1];
  b32 length;  
} NumberInputField;

typedef struct cursor_settings {
  f32 cursorTimer;
  bool cursorVisible;
  f32 cursor_ON_time;
  f32 cursor_OFF_time;
} cursor_settings;

void update_number_input_field(NumberInputField *element, cursor_settings *cs);
void draw_number_input_field(NumberInputField *element, cursor_settings *cs);