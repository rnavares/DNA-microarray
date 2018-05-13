#makefile

CFLAGS = -ansi -Wall -pedantic

COMPILER = gcc
IDIRS = -I /usr/include
LDIRS = -L /usr/lib
LDIRS = -L .


all: main_euler main_TSP

#objects
pq.o: pq.c pq.h
	$(COMPILER) $(CFLAGS) $(IDIRS) -c -g pq.c

graph.o: graph.c graph.h
	$(COMPILER) $(CFLAGS) $(IDIRS) -c -g graph.c

microarray.o: microarray.c microarray.h
	$(COMPILER) $(CFLAGS) $(IDIRS) -c -g microarray.c

main_euler.o: main_euler.c pq.h graph.h microarray.h
	$(COMPILER) $(CFLAGS) $(IDIRS) -c -g main_euler.c

main_TSP.o: main_TSP.c pq.h graph.h microarray.h
	$(COMPILER) $(CFLAGS) $(IDIRS) -c -g main_TSP.c

#executables
main_euler: main_euler.o pq.o pq.h graph.o graph.h microarray.h microarray.o
	$(COMPILER) -o main_euler main_euler.o pq.o graph.o microarray.o $(LDIRS) -lm

main_TSP: main_TSP.o pq.o pq.h graph.o graph.h microarray.h microarray.o
	$(COMPILER) -o main_TSP main_TSP.o pq.o graph.o microarray.o $(LDIRS) -lm

clean:
	rm -fr core *.o main_euler main_TSP *~ 	
#end  MAKEFILE
