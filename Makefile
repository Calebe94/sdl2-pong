##
# First Game
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

exported_functions="'__invoke_i32_$struct_FT_CMap_ClassRec_*_i8*_$struct_FT_CharMapRec_*_$struct_FT_CMapRec_**'," \
"'__invoke_i32_$struct_FT_Outline_*_$struct_FT_Outline_Funcs_*_i8*'," \
"'__invoke_i32_i8*_$struct_FT_ValidatorRec_*'," \
"'__invoke_void_$struct_FT_ValidatorRec_*_i8*_i8*_i32'," \
"'__invoke_void_$struct_gray_TWorker_*'," \
"'emscripten_longjmp_jmpbuf'"

OUTPUT_DIR=bin/
BIN=bin/game

${OUTPUT_DIR}:
	@mkdir -p ${OUTPUT_DIR}

all: ${OUTPUT_DIR}
	$(CC) $(FLAGS) $(SOURCE) $(LIBS) -o $(BIN)

run: all
	$(BIN)

web:
	emcc $(SOURCE) -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s WASM=2 --preload-file "8-BIT_WONDER.ttf" $(FLAGS) -o $(TITLE).html
	#emcc $(wildcard *.o) -o sdl_2_0_sample.html --bind -s FETCH=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS='["png"]' -s WASM=2 --preload-file "8-BIT_WONDER.ttf" -s ERROR_ON_UNDEFINED_SYMBOLS=1 -s LLD_REPORT_UNDEFINED -s EXPORTED_FUNCTIONS="['_main', '_malloc']" $(FLAGS) -s EXPORTED_FUNCTIONS="['_main', '_malloc']"



.PHONY: ${OUTPUT_DIR} web
# end
