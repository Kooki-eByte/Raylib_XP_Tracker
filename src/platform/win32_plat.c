#ifdef _WIN32

#include "platform.h"
#include <windows.h>

bool create_directory() {
  if (CreateDirectory("save_data", 0)) {
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