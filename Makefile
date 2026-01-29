PROJ = test
INCLUDES = ${shell sdl2-config --cflags}
LIBS = 
LIBRARIES = ${shell sdl2-config --libs}

run: $(PROJ).exe
	./$<

.o.cpp:
	g++ ${INCLUDES} -c $c -o  $@

clean:
	rm -R build/lib/*

$(PROJ).exe: $(PROJ).cpp 
	g++ ${INCLUDES} -Llibfunction/build/lib $^ -o $@ -Llibfunction
# The ^ does everything above, the < does just the $(PROJ)