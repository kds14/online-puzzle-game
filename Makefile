TARGET=./build/trl
SOURCES=./src/*.cpp
CC=g++
FLAGS=-std=c++17 -g -Wall -Werror

all: $(TARGET)

$(TARGET):$(SOURCES)
	mkdir -p build
	$(CC) $(FLAGS) -o $@ $^ `sdl2-config --cflags --libs`

clean:
	rm -rf ./build
