CC=g++
#-g = Debugginginformationen einf�gen; -Wall = alle Warnings anzeigen
CFLAGS=-g -std=c++11 -fopenmp -lpthread

#ben�tigt sortnames um Ziel (target) all zu erreichen make oder make all
all: shortestPath 

shortestPath: shortestPath.o
	${CC} ${CFLAGS} -o shortestPath.exe shortestPath.o

shortestPath.o: shortestPath.cpp
	${CC} ${CFLAGS} -o shortestPath.o -c shortestPath.cpp

#l�scht alles unben�tigte
clean:
	rm -f shortestPath.exe shortestPath.o