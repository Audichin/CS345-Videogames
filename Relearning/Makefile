PROG = test

run: $(PROG).exe
	./$<

clean:
	rm -f $(PROG) *.o

.0.cpp:
	g++ -c $< o $.o

$(PROG).exe: $(PROG).cpp functions.o4r
	g++ $^ -o $@
# 	The ^ does everything above, the < does just the $(PROG)
