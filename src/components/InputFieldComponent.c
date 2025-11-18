#include "InputFieldComponent.h"

void update_number_input_field(NumberInputField *element, cursor_settings *cs) {
  // Focus check
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse_pos = GetMousePosition();
    element->isFocused = CheckCollisionPointRec(mouse_pos, element->bounds);
  }

  if (element->isFocused) {
    b32 key_char = GetCharPressed();
    while (key_char > 0) {
      if ((key_char >= '0') && (key_char <= '9') &&
          (element->length < MAX_NUMBER_INPUT)) {
        element->value[element->length] = (char)key_char;
        element->length++;
        element->value[element->length] = '\0';
      }
      key_char = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && element->length > 0) {
      element->length--;
      element->value[element->length] = '\0';
    }
    // TODO:
    // - log for cursor position
    // - logic to be able to delete char based on cursor position

    // Cursor blink
    cs->cursorTimer += GetFrameTime();
    if (cs->cursorVisible && cs->cursorTimer >= cs->cursor_ON_time) {
      cs->cursorVisible = false;
      cs->cursorTimer = 0.0f;
    } else if (!cs->cursorVisible && cs->cursorTimer >= cs->cursor_OFF_time) {
      cs->cursorVisible = true;
      cs->cursorTimer = 0.0f;
    }
  }
}

void draw_number_input_field(NumberInputField *element, cursor_settings *cs) {
  const b32 Y_AXIS_OFFSET = 15;
  const b32 X_AXIS_OFFSET = 10;
  const b32 TEXT_FONT_SIZE = 20;

  DrawRectangleLinesEx(element->bounds, 2, element->isFocused ? BLUE : GRAY);
  DrawText(element->value, element->bounds.x + X_AXIS_OFFSET,
           element->bounds.y + Y_AXIS_OFFSET, TEXT_FONT_SIZE, DARKGRAY);

  if (element->isFocused && cs->cursorVisible) {
    b32 text_width = MeasureText(element->value, TEXT_FONT_SIZE);
    b32 cursor_width = 2;
    b32 cursor_height = TEXT_FONT_SIZE;
    DrawRectangle(element->bounds.x + X_AXIS_OFFSET + text_width,
                  element->bounds.y + Y_AXIS_OFFSET, cursor_width,
                  cursor_height, BLACK);
  }
}

void update_text_input_field(TextInputField *element, cursor_settings *cs) {
  // Focus check
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse_pos = GetMousePosition();
    element->isFocused = CheckCollisionPointRec(mouse_pos, element->bounds);
  }

  if (element->isFocused) {
    b32 key_char = GetCharPressed();
    while (key_char > 0) {
      if ((element->length < MAX_TEXT_SIZE)) {
        element->value[element->length] = (char)key_char;
        element->length++;
        element->value[element->length] = '\0';
      }
      key_char = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && element->length > 0) {
      element->length--;
      element->value[element->length] = '\0';
    }
    // TODO:
    // - log for cursor position
    // - logic to be able to delete char based on cursor position

    // Cursor blink
    cs->cursorTimer += GetFrameTime();
    if (cs->cursorVisible && cs->cursorTimer >= cs->cursor_ON_time) {
      cs->cursorVisible = false;
      cs->cursorTimer = 0.0f;
    } else if (!cs->cursorVisible && cs->cursorTimer >= cs->cursor_OFF_time) {
      cs->cursorVisible = true;
      cs->cursorTimer = 0.0f;
    }
  }
}

void draw_text_input_field(TextInputField *element, cursor_settings *cs) {
  const b32 Y_AXIS_OFFSET = 15;
  const b32 X_AXIS_OFFSET = 10;
  const b32 TEXT_FONT_SIZE = 16;

  DrawRectangleLinesEx(element->bounds, 2, element->isFocused ? BLUE : GRAY);
  DrawText(element->value, element->bounds.x + X_AXIS_OFFSET,
           element->bounds.y + Y_AXIS_OFFSET, TEXT_FONT_SIZE, DARKGRAY);

  if (element->isFocused && cs->cursorVisible) {
    b32 text_width = MeasureText(element->value, TEXT_FONT_SIZE);
    b32 cursor_width = 2;
    b32 cursor_height = TEXT_FONT_SIZE;
    DrawRectangle(element->bounds.x + X_AXIS_OFFSET + text_width,
                  element->bounds.y + Y_AXIS_OFFSET, cursor_width,
                  cursor_height, BLACK);
  }
}