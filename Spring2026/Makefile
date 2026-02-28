PROJ=simple
FLAGS=$(shell sdl2-config --cflags) -g
LIBS=$(shell sdl2-config --libs)

run: ${PROJ}.exe
	./$<

${PROJ}.exe: ${PROJ}.cpp Character.hpp Game.hpp MediaManager.hpp MyGame.hpp
	g++ ${FLAGS} $< -o $@ ${LIBS}

clean:
	rm ${PROJ}.exe