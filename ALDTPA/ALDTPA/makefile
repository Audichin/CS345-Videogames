# BUILD ONLY FROM MSYS2 MINGW64 SHELL

CXX = C:/msys64/mingw64/bin/g++

FLAGS = -std=c++17 -Wall \
        -IC:/msys64/mingw64/include \
        -IC:/msys64/mingw64/include/SDL2 \
        -IC:/msys64/mingw64/include/SDL2/SDL_mixer \
        -Dmain=SDL_main

LIBS = -LC:/msys64/mingw64/lib \
       -lmingw32 \
       -lSDL2main \
       -lSDL2 \
       -lSDL2_mixer \
       -lcurl \
       -lws2_32 -lcrypt32 -lbcrypt \
       -liphlpapi -lsecur32 -lwldap32 \
       -pthread

PROJ = gamewindow.cpp

$(PROJ).exe: gamewindow.cpp gamewindow.hpp phyphox.hpp character.hpp mediamanager.hpp
	$(CXX) $< -o $@ $(FLAGS) $(LIBS)

run: $(PROJ).exe
	./$(PROJ).exe

clean:
	rm -f $(PROJ).exe *.o
