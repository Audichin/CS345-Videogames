#FLAGS =  $(shell sdl2-config --cflags) -g
FLAGS = -IC:\msys64\mingw64\include\SDL2

#LIBS = $(shell sdl2-config --libs)
LIBS  = -LC:\msys64\mingw64\lib -lmingw32 -lSDL2main -lSDL2
PROJ = simple

simple.exe: simple.cpp
	g++ $(FLAGS) $< -o $@ $(LIBS)

run: simple.exe
	./$<
