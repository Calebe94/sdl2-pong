##
# First Game
#
# @file
# @version 0.1

CC=gcc
SOURCE=$(wildcard src/*.c)
SDL2_FLAGS=`$$(which sdl2-config) --cflags --libs`
FLAGS=-Wall -Wextra -std=c99
FLAGS+=-fanalyzer
LIBS='-Wl,-rpath,$$ORIGIN' $(SDL2_FLAGS) -l SDL2_image -l SDL2_mixer -l SDL2_ttf

OUTPUT_DIR=bin/
BIN=bin/game

${OUTPUT_DIR}:
	@mkdir -p ${OUTPUT_DIR}

all: ${OUTPUT_DIR}
	$(CC) $(FLAGS) $(SOURCE) $(LIBS) -o $(BIN)

run: all
	$(BIN)

.PHONY: ${OUTPUT_DIR}
# end
