#include "data_persistence.h"

bool save_data(user_project_data user_data) {
  char json_ext[] = ".dat";
  char *project_file_name = strcat(user_data.project_name, json_ext);

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

// user_project_data load_data(char *project) {};