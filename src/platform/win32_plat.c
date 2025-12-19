#ifdef _WIN32
#include <windows.h>

#include "platform.h"

char *get_save_dir(const char *appname) {
  fprintf(stderr, "get_save_dir is called\n");
  char *path = (char *)malloc(sizeof(appname));

  fprintf(stderr, "about to call strcpy for char * null val src\n");
  strcpy(path, appname);
  fprintf(stderr, "cpy string to path\n name of string %s\n", path);
  return path;
}

bool create_directory() {
  if (CreateDirectory(get_save_dir("save_data"), 0)) {
    printf("Successfully created directory!\n");
    return true;
  } else {
    DWORD err = GetLastError();
    if (err == ERROR_ALREADY_EXISTS) {
      fprintf(stderr, "Directory already exists!\n");
      return true;
    } else {
      fprintf(stderr, "Failed to create directory!\n");
      return false;
    }
  }
}

#endif