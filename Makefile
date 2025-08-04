SRC_CORE := $(wildcard ./src/*.c) $(wildcard ./src/**/*.c)
OUTPUT := ./bin/main

# Compiler Settings
CC := gcc
C_STD := c99
CFLAGS := -Wall -Werror -Wimplicit-function-declaration
DEBUG_FLAG := -g

# raylib link flags
RAYLIB_FLAGS := -lraylib -lgdi32 -lwinmm
RAYLIB_LINUX_FLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	$(CC) -std=$(C_STD) $(DEBUG_FLAG) $(SRC_CORE) -o $(OUTPUT) $(CFLAGS) $(RAYLIB_LINUX_FLAGS)

# Command that needs to run is this:
# gcc -std=c99 -o main main.c -Wall -Werror -Wimplicit-function-declaration -Llib -lraylib -lgdi32 -lwinmm

clean:
	rm ./bin/main
