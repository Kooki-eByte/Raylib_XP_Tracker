#include "data_persistence.h"

static char *get_full_path(char *filename) {
  char *filepath = "save_data/";

  char *full_path_to_file = (char *)malloc(1 + strlen(filepath) + strlen(filename));

  strcpy(full_path_to_file, filepath);
  strcat(full_path_to_file, filename);

  return full_path_to_file;
}

bool save_data(user_project_data *user_data) {
  TraceLog(LOG_DEBUG, "Attempting to save game!");

  bool success = false;

  FILE *fileData = fopen(get_full_path(user_data->project_name), "wb");

  if (fileData != NULL) {
    // handle changes to already existing save data
    b32 is_Written = fwrite(user_data, sizeof(user_project_data), 1, fileData);
    
    if (is_Written == 1) {
      success = true;
    }
    fclose(fileData);
  }

  return success == 1 ? true : false;
}

void load_data(user_project_data *user_data) {
  char data_ext[5] = ".dat";
  char *project_file_name = strcat(user_data->project_name, data_ext);

  TraceLog(LOG_DEBUG, "Attempting to load game data");

  FILE *fileData = fopen(get_full_path(project_file_name), "rb");

  if (fileData != NULL) {
    fseek(fileData, 0, SEEK_END);
    long size = ftell(fileData);
    rewind(fileData);

    TraceLog(LOG_DEBUG, "File size: %ld bytes, struct size: %zu bytes", size, sizeof(user_project_data));

    size_t num_of_instance = 1;
    size_t read_count = fread(user_data, sizeof(user_project_data), num_of_instance, fileData);

    if (read_count != 1) {
      TraceLog(LOG_TRACE, "Fread failed to read data!");
      TraceLog(LOG_DEBUG, "Expected to read %zu bytes, actually read %zu", sizeof(user_project_data), read_count * sizeof(user_project_data));
    }

    TraceLog(LOG_DEBUG, "Want to read %zu bytes, actually read %zu bytes", sizeof(user_project_data), read_count * sizeof(user_project_data));

    fclose(fileData);

    TraceLog(LOG_DEBUG, "This is the user data:\n project name: %s\n current lvl: %d\n current exp: %.2f\n max exp: %.2f\n",
      user_data->project_name,
      user_data->user_level,
      user_data->current_exp,
      user_data->max_exp);
  }
}

u8 delete_data(char *fileName) {
  char *full_path_to_file = get_full_path(fileName);
  
  // printf("This is being ran with this file [%s]\n", full_path_to_file);
  if (!remove(full_path_to_file)) {
    printf("File [%s] deleted successfully!\n", fileName);
    return true;
  } else {
    printf("[ERR]: Unable to delete the file [%s].\n", fileName);
    return false;
  }
}