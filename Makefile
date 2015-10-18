poly.out: Source.o polygon.o
	g++ -ansi -g -o poly.out Source.o polygon.o -lglut -lGL

Source.o : Source.cpp polygon.h
	g++ -ansi -g -c Source.cpp

polygon.o : polygon.cpp polygon.h
	g++ -ansi -g -c polygon.cpp

clean : 
	rm -f poly.out Source.o
