SRC_CORE := $(wildcard ./src/*.c) $(wildcard ./src/**/*.c)
OUTPUT_BIN := ./bin
TARGET_NAME := xp_tracker

CC := gcc
CFLAGS := -Wall -Werror -Wimplicit-function-declaration -std=c99

WINDOWS_EXE := $(OUTPUT_BIN)/$(TARGET_NAME).exe
LINUX_EXE := $(OUTPUT_BIN)/$(TARGET_NAME)

RAYLIB_LIBS :=

# ------------------------ PLATFORM DETECTION ------------------------
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    PLATFORM := WINDOWS
else ifeq ($(UNAME_S),Linux)
    PLATFORM := LINUX
endif

# ------------------------ WINDOWS ------------------------
ifeq ($(PLATFORM),WINDOWS)

EXE := $(WINDOWS_EXE)

LOCAL_INC := $(wildcard ./include)
LOCAL_LIB := $(wildcard ./lib)

ifeq ($(LOCAL_INC),)
    INCLUDE_PATH := -I/mingw64/include
else
    INCLUDE_PATH := -I./include
endif

ifeq ($(LOCAL_LIB),)
    LIB_PATH := -L/mingw64/lib
    RAYLIB_LIBS += -lraylib -lglfw3
else
    LIB_PATH := -L./lib
    RAYLIB_LIBS += -lraylib
endif

RAYLIB_LIBS += \
    -lopengl32 \
    -lgdi32 \
    -lwinmm \
    -lcomdlg32 \
    -lole32 \
    -luuid \
    -lkernel32 \
    -limm32 \
    -lws2_32 \
    -lcomctl32 \
    -lshlwapi \
    -luser32

$(EXE): $(SRC_CORE)
	$(CC) $(INCLUDE_PATH) $(LIB_PATH) $(SRC_CORE) -o $@ $(CFLAGS) $(RAYLIB_LIBS)

endif

# ------------------------ LINUX ------------------------
ifeq ($(PLATFORM),LINUX)

EXE := $(LINUX_EXE)

# Arch/Ubuntu raylib package installation:
#   sudo pacman -S raylib    (Arch)
#   sudo apt install libraylib-dev   (Ubuntu)

INCLUDE_PATH :=
LIB_PATH :=

# Linux raylib + dependencies
RAYLIB_LIBS += \
    -lraylib \
    -lGL \
    -lm \
    -lpthread \
    -ldl \
    -lrt \
    -lX11

$(EXE): $(SRC_CORE)
	$(CC) $(INCLUDE_PATH) $(LIB_PATH) $(SRC_CORE) -o $@ $(CFLAGS) $(RAYLIB_LIBS)

endif

# ----------------------- TARGETS -------------------------
.PHONY: all clean dirs

all: dirs $(EXE)

dirs:
	mkdir -p $(OUTPUT_BIN)

clean:
	rm -rf $(OUTPUT_BIN)/*
