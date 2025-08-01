#include "data_persistence.h"

char data_ext[] = ".dat";

bool save_data(user_project_data user_data) {
  char data_ext[] = ".dat";
  char *project_file_name = strcat(user_data.project_name, data_ext);

  TraceLog(LOG_INFO, "Attempting to save game!");

  bool success = false;

  TraceLog(LOG_INFO, "Trying to LoadFileData");
  FILE *fileData = fopen(project_file_name, "wb");

  if (fileData != NULL) {
    // handle changes to already existing save data
    b32 is_Written = fwrite(&user_data, sizeof(user_project_data), 1, fileData);
    
    if (is_Written == 1) {
      success = true;
    }
    fclose(fileData);
  }

  return success == 1 ? true : false;
}

// FIXME: Not loading in game data!
void load_data(user_project_data user_data) {
  char data_ext[] = ".dat";
  char *project_file_name = strcat(user_data.project_name, data_ext);

  TraceLog(LOG_INFO, "Attempting to load game data");

  FILE *fileData = fopen(project_file_name, "rb");


  if (fileData != NULL) {
    size_t num_of_instance = 1;
    size_t data_read = fread(&user_data, sizeof(user_project_data), num_of_instance, fileData);

    if (data_read != 1) {
      TraceLog(LOG_ERROR, "Failed to read data from file.");
      fclose(fileData);
    }

    fclose(fileData);
  }
}