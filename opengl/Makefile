LDFLAGS=-lglut -lGL -lGLU
CFLAGS=-g -c
CC=gcc
BI=pr12 pr13 pr23 pr25 pr26 pr27 pr210 pr211 pr213 pr31 pr32 pr35 pr36 pr37
#OBJ:=$(wildcard *.o)
#OBJ=*.o
#VPATH=ish

all:$(BI)

pr12:$(LDFLAGS) pr12.o
pr12.o:$(LDFLAGS) pr12.c
#	$(CC) $(CFLAGS) ish/pr12.c -o pr12.o

pr13:$(LDFLAGS) pr13.o
pr13.o:

pr23:$(LDFLAGS) pr23.o

pr25:$(LDFLAGS) pr25.o

pr26:$(LDFLAGS) pr26.o

pr27:$(LDFLAGS) pr27.o

pr210:$(LDFLAGS) pr210.o

pr211:$(LDFLAGS) pr211.o

pr213:$(LDFLAGS) -lm pr213.o

pr31:$(LDFLAGS) pr31.o

pr32:$(LDFLAGS) pr32.o

pr35:$(LDFLAGS) pr35.o

pr36:$(LDFLAGS) pr36.o

pr37:$(LDFLAGS) pr37.o

clean:
	rm -f *.o $(BI)

clean_all:
	rm -f *.o $(BI) *.c~
