# BUILD ONLY FROM MSYS2 MINGW64 SHELL

CXX = g++

FLAGS = -std=c++17 -Wall \
        -IC:/msys64/mingw64/include/SDL2 \
        -Dmain=SDL_main

LIBS = -LC:/msys64/mingw64/lib \
       -lmingw32 \
       -lSDL2main \
       -lSDL2 \
       -lcurl \
       -lws2_32 -lcrypt32 -lbcrypt \
       -liphlpapi -lsecur32 -lwldap32 \
       -pthread

PROJ = gamewindow.cpp

$(PROJ).exe: gamewindow.cpp gamewindow.h phyphox.h 
	$(CXX) $< -o $@ $(FLAGS) $(LIBS)

run: $(PROJ).exe
	./$(PROJ).exe

clean:
	rm -f $(PROJ).exe *.o
        
# game: 
#         $(PROJ).exe gamewindow.cpp gamewindow.h phyphox.h 
#         $(CXX) $< -o $@ $(FLAGS) $(LIBS)
#         ./$(PROJ).exe
