CXXFLAGS= -Wall -g # -O2
LDFLAGS=-lm `sdl-config --libs`

CXX=g++

BINAR=u174


all:$(BINAR)

u174:u174.o
	$(CXX) u174.o -g -lSDL -lSDL_image  -o u174



.PHONY: clean_old clean

clean:
	rm -f *.o $(BINAR)

clean_old:
	rm -f *~

clean_all: clean clean_old
