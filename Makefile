CC=gcc
SRC=src
TEST=test
SRCS=$(wildcard $(SRC)/*.cpp)
TESTS=$(wildcard $(TEST)/*.cpp)
SRCS_NOT_MAIN=$(filter-out $(SRC)/main.cpp, $(wildcard $(SRC)/*.cpp))

sdl2-config = $$(sdl2-config --cflags --libs)

all:
	$(CC) -o main main.c $(sdl2-config)

