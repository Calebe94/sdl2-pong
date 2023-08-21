##
# SDL2 Pong Game
#
# @file
# @version 0.1

CC=gcc
EMCC=emcc
TITLE=sdl2-pong
SOURCE=$(wildcard src/*.c)
SDL2_FLAGS=`$$(which sdl2-config) --cflags --libs`
FLAGS=-Wall -Wextra -std=c99
# FLAGS+=-fanalyzer
LIBS='-Wl,-rpath,$$ORIGIN' $(SDL2_FLAGS) -l SDL2_image -l SDL2_mixer -l SDL2_ttf

OUTPUT_DIR=bin/
WASM_DIR=wasm/
BIN=bin/game

${WASM_DIR}:
	@mkdir -p ${WASM_DIR}

${OUTPUT_DIR}:
	@mkdir -p ${OUTPUT_DIR}

all: ${OUTPUT_DIR}
	$(CC) $(FLAGS) $(SOURCE) $(LIBS) -o $(BIN)

run: all
	$(BIN)

web: ${WASM_DIR}
	${EMCC} ${SOURCE} --shell-file wasm/index_shell.html -o ${WASM_DIR}/index.html -s WASM=2 -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s USE_WEBGL2=1 -s SDL2_IMAGE_FORMATS='["png"]'\
    $(FLAGS) \
    -I/usr/include/SDL2 \
    -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf \
    --preload-file "Pixeboy.ttf"

web-run: web
	python3 -m http.server -d ${WASM_DIR}

.PHONY: ${OUTPUT_DIR} web
# end
