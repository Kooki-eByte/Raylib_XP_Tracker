#include "data_persistence.h"

bool save_data(const UserProjectData *user_data)
{
  TraceLog(LOG_DEBUG, "Attempting to save game!");

  bool success = false;

  FILE *fileData = fopen(user_data->project_name, "wb");

  if (fileData != NULL)
  {
    // handle changes to already existing save data
    b32 is_Written = fwrite(user_data, sizeof(UserProjectData), 1, fileData);

    if (is_Written == 1)
    {
      success = true;
    }
    fclose(fileData);
  }

  return success == 1 ? true : false;
}

void load_data(UserProjectData *user_data)
{
  char data_ext[5] = ".dat";
  char *project_file_name = strcat(user_data->project_name, data_ext);

  TraceLog(LOG_DEBUG, "Attempting to load game data");

  FILE *fileData = fopen(project_file_name, "rb");

  if (fileData != NULL)
  {
    fseek(fileData, 0, SEEK_END);
    long size = ftell(fileData);
    rewind(fileData);

    TraceLog(LOG_DEBUG, "File size: %ld bytes, struct size: %zu bytes", size, sizeof(UserProjectData));

    size_t num_of_instance = 1;
    size_t read_count = fread(user_data, sizeof(UserProjectData), num_of_instance, fileData);

    if (read_count != 1)
    {
      TraceLog(LOG_TRACE, "Fread failed to read data!");
      TraceLog(LOG_DEBUG, "Expected to read %zu bytes, actually read %zu", sizeof(UserProjectData), read_count * sizeof(UserProjectData));
    }

    TraceLog(LOG_DEBUG, "Want to read %zu bytes, actually read %zu bytes", sizeof(UserProjectData), read_count * sizeof(UserProjectData));

    fclose(fileData);

    TraceLog(LOG_DEBUG, "This is the user data:\n project name: %s\n current lvl: %d\n current exp: %.2f\n max exp: %.2f\n",
             user_data->project_name,
             user_data->user_level,
             user_data->current_exp,
             user_data->max_exp);
  }
}