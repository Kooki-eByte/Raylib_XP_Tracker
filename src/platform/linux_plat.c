#ifdef __linux__

#include "platform.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

#define XP_TRACKER_LOC "xp_tracker"
#define XP_TRACKER_SAVE_DATA_LOC "xp_tracker/save_data"

char *get_save_dir(const char *appname) {
  const char *xdg = getenv("XDG_DATA_HOME");
  const char *home = getenv("HOME");
  char *path = NULL;
  
  if (xdg != NULL && xdg[0] != '\0') {
    // TODO: Check if this works with linux side
    size_t len = strlen(xdg) + strlen("xp_tracker/") + strlen(appname) + 2;
    path = malloc(len);
    snprintf(path, len, "%s%s%s", xdg, "xp_tracker/",appname);
  
  } else if (home != NULL && home[0] != '\0') {
  
    size_t len = strlen(home) + strlen(appname) + 20;
    path = malloc(len);
    snprintf(path, len, "%s/.local/share/%s", home, appname);

  }

  return path;
}

bool create_directory() {
  struct stat st = {0};

  char *dir = get_save_dir(XP_TRACKER_LOC);

  if (!dir) {
    fprintf(stderr, "could not determine directory\n");
    free(dir);
    return false;
  }

  if (stat(dir, &st) == -1) {
    if (mkdir(dir, 0700) == -1) {
      int err_msg = errno;
      fprintf(stderr, "Failed to create xp tracker directory at location: %s - err code: %d\n", XP_TRACKER_LOC, err_msg);
      
      if (errno) {
        fprintf(stderr, "The parent directory does not allow write permission to the process, or one of the directories in path did not allow search permission. Err Message code: %d\n", err_msg);
      }

      free(dir);
      return false;
    }
  }
  
  char *save_data_dir = get_save_dir(XP_TRACKER_SAVE_DATA_LOC);
  if (stat(save_data_dir, &st) == -1) {
    if (mkdir(save_data_dir, 0700) == -1) {
      fprintf(stderr, "Failed to create save_data directory at location: %s\n", XP_TRACKER_SAVE_DATA_LOC);

      free(dir);
      free(save_data_dir);
      return false;
    }
  }
  

  free(dir);
  free(save_data_dir);
  return true;
}

#endif