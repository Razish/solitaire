# - Linux Makefile
# Compiler: gcc
# Platform: Linux i386
# Requires: libsdl-image1.2-dev libsdl1.2-dev
# Project: solitaire

CC = gcc
TARGET = ../bin/solitaire

# Compiler Flags - swap CONFIG for release/debug builds
#FLAGS += -O2 -DNDEBUG
FLAGS += -g3 -D_DEBUG
FLAGS += -I./
FLAGS += -D__GCC__
FLAGS += -Wall
FLAGS += -m32 -msse3

# Files
FILES += main.o
FILES += game/game.o
FILES += input/input.o
FILES += renderer/renderer.o renderer/texture.o
FILES += utils/math.o utils/string.o utils/hash.o

#Libraries
LIBS = -lGL -lSDL -lSDLmain -lSDL_image -lz -lpng -lpng12 -ljpeg

# "make"
$(TARGET):  $(FILES)
	$(CC) -o $(TARGET) $(FILES) $(LIBS)
	chmod +x $(TARGET)

# "make .c.o"
.c.o:
	$(CC) $(FLAGS) -c $*.c -o $*.o

# "make clean"
clean:
	rm -f $(FILES) $(TARGET)

