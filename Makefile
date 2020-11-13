source=Dijkstra.c
cc=gcc
cflags = -Wall -std=c11 -pedantic -ggdb
comp=Dijkstra


.PHONY: all
all: $(comp) 
clean: 
	rm -f *.o $(comp) 

$(comp):$(source)
	$(cc) $(cflags) $(source) -o $(comp)