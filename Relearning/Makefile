PROG = test

run: $(PROG).exe
	./$<

clean:
	rm -f $(PROG) *.o

$(PROG).exe: $(PROG).cpp
	g++ $< -o $@
