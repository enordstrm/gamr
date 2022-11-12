OBJS = main.c

CC = gcc

COMPILER_FLAGS = -w

LINKER_FLAGS =

OBJ_NAME = gamr

BUILD_PATH = build

OBJ_PATH = $(BUILD_PATH)/$(OBJ_NAME)

OBJS = main.c

SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf

debug : COMPILER_FLAGS += -DDEBUG -g
debug : all

build_path :
	mkdir -p $(BUILD_PATH)

all : $(OBJS) build_path
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(SDL_FLAGS) -o $(OBJ_PATH)

run : all
	./$(OBJ_PATH)
