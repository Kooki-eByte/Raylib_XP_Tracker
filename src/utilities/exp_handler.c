#include "exp_handler.h"

static const f32 gain_percentage = 0.08f;
static const b32 hours_per_day = 24;
static const b32 minutes_per_hour = 60;

static f32 updateMaxExp(b32 level)
{
  f32 base_exp = 100.0f;
  f32 growth_rate = 1.25f;
  return base_exp + powf(growth_rate, level - 1);
}

char *getExpPercentage(f32 max_exp, f32 current_exp)
{
  f32 exp_percent_remaining = current_exp / max_exp * 100;

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

bool gainHourExp(b32 hours_spent, f32 *exp, f32 *current_max_exp)
{
  if (hours_spent <= 0)
  {
    return false;
  }

  // Formula for exp gained
  b32 modifier = 8;
  f32 exp_gained = (*current_max_exp * gain_percentage) + (hours_spent / hours_per_day) + modifier;
  TraceLog(LOG_INFO, "This is the current exp gained for hours: %.2f", exp_gained);

  f32 temp_exp_hold = *exp + exp_gained;

  if (temp_exp_hold >= *current_max_exp)
  {
    *exp = fmodf(temp_exp_hold, *current_max_exp);
    *current_max_exp *= 2;
    return true;
  }

  *exp = temp_exp_hold;
  return false;
}

bool gainMinuteExp(b32 minutes_spent, f32 *exp, f32 *current_max_exp)
{
  if (minutes_spent <= 0)
    return false;

  // Formula for exp gained
  f32 exp_gained = (*current_max_exp * gain_percentage) + ((minutes_spent / minutes_per_hour) / hours_per_day);
  TraceLog(LOG_INFO, "This is the current exp gained for minutes: %.2f", exp_gained);

  f32 temp_exp_hold = *exp + exp_gained;

  if (temp_exp_hold >= *current_max_exp)
  {
    *exp = fmodf(temp_exp_hold, *current_max_exp);
    *current_max_exp *= 2;
    return true;
  }

  *exp = temp_exp_hold;
  return false;
}

void handleLevelUp(UserProjectData *user_data)
{
  user_data->user_level += 1;
  user_data->max_exp = updateMaxExp(user_data->user_level);
}