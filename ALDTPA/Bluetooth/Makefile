CXX = g++
LIBS = -std=c++17 -I$(HOME)/vcpkg/installed/x64-linux/include/**
FLAGS = -L$(HOME)/vcpkg/installed/x64-linux/include/** -lcurl
PROJ = phyphox

phyphox.exe: phyphox.cpp
	g++ $(FLAGS) $< -o $@ $(LIBS)

run: $(PROJ)
	./$<