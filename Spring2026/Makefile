FLAGS = $(shell sdl2-config --cflags)
LIBS  = $(shell sdl2-config --libs)
PROJ = simple

simple.exe: simple.cpp
	g++ $(FLAGS) $< -o $@ $(LIBS)

run: $(PROJ)
	./$<