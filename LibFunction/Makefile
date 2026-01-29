PROJ = functions

./build/lib${PROJ}.so: src /${PROJ}.cpp
	g++ -fPIC - linclude -shared $^ -o $@

clean:
	rm -f $(PROJ) *.o


