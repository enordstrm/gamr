OBJS = main.c

CC = gcc

COMPILER_FLAGS = -w

LINKER_FLAGS =

OBJ_NAME = gamr

OBJS = main.c

SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf

debug : COMPILER_FLAGS += -DDEBUG -g
debug : all

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(SDL_FLAGS) -o $(OBJ_NAME)

run : all
	./$(OBJ_NAME)
