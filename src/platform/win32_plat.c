#ifdef _WIN32

#include "platform.h"
#include <windows.h>

char *get_save_dir(const char *appname) {
  char * path = NULL;

  strcpy(path, appname);

  return path;
}

bool create_directory() {
  if (CreateDirectory(get_save_dir("save_data"), 0)) {
    printf("Successfully created directory!\n");
    return true;
  } else {
    DWORD err = GetLastError();
    if (err == ERROR_ALREADY_EXISTS) {
      printf("Directory already exists!\n");
      return true;
    } else {
      fprintf(stderr, "Failed to create directory!\n");
      return false;
    }
  }
}

#endif