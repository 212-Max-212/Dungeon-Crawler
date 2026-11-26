# mingw32-make to compile the project on Windows
# compiler
CC = gcc

# compiling parameters
# -Wall   --- enable common warnings
# -Wextra --- enable additional warnings
# -g      --- debugger info
CFLAGS = -Wall -Wextra -g

# var with all files .c (also in the subfolders map/, npc/, player/)
SRC = $(wildcard src/*.c src/*/*.c)

# var with all files .h
HEADERS = $(wildcard src/*.h src/*/*.h)

# name of file: files.c files.h param -o name of file
dungeon.exe: $(SRC) $(HEADERS) $(CC) $(CFLAGS) $(SRC) -o dungeon.exe
