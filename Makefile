SRC_CORE := $(wildcard ./src/*.c) $(wildcard ./src/**/*.c)
OUTPUT_BIN := ./bin
LINUX_APP := $(OUTPUT_BIN)/xp_tracker
WINDOWS_APP := $(OUTPUT_BIN)/xp_tracker.exe

# Compiler Settings
CC := gcc
C_STD := c99
CFLAGS := -Wall -Werror -Wimplicit-function-declaration
DEBUG_FLAG := -g

# OS-specific settings
ifeq ($(OS),Windows_NT)
RAYLIB_FLAGS := -lraylib -lgdi32 -lwinmm
LIB := -Llib
RAYLIB_DLL := $(LIB)
MAIN_APP_TARGET := $(WINDOWS_APP)
else
# Linux
RAYLIB_FLAGS := -lraylib -lgdi32 -lwinmm
RAYLIB_LINUX_FLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
MAIN_APP_TARGET := $(LINUX_APP)
endif

all: $(MAIN_APP_TARGET)

$(LINUX_APP):
	$(CC) -std=$(C_STD) $(DEBUG_FLAG) $(SRC_CORE) -o $(MAIN_APP_TARGET) $(CFLAGS) $(RAYLIB_LINUX_FLAGS)

$(WINDOWS_APP):
	$(CC) -std=$(C_STD) $(DEBUG_FLAG) $(SRC_CORE) -o $(MAIN_APP_TARGET) $(CFLAGS) $(RAYLIB_DLL) $(RAYLIB_FLAGS)

#clean:
#	rm -rf $(OUTPUT_BIN)/main.exe
