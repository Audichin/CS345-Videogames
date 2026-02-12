CXX = g++
# LIBS = -std=c++17 -I$(HOME)/vcpkg/installed/x64-linux/include/**
# FLAGS = -L$(HOME)/vcpkg/installed/x64-linux/include/** -lcurl
FLAGS = $(shell sdl2-config --cflags)
LIBS  = $(shell sdl2-config --libs) -lcurl 
PROJ = phyphox

phyphox.exe: phyphox.cpp
	g++ $(FLAGS) $< -o $@ $(LIBS)

run: $(PROJ)
	./$<