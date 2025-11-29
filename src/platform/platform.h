#ifndef KOOKIE_PLATFORM_H
#define KOOKIE_PLATFORM_H

#include "../defines.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

extern bool create_directory();
extern char *get_save_dir(const char* appname);

#endif // KOOKIE_PLATFORM_H