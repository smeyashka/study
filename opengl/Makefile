CFLAGS=-g -Wall
LDFLAGS=-lglut -lGL -lGLU -lm
CC=gcc
BI=pr12 pr13 pr23 pr25 pr26 pr27 pr210 pr211 pr213 pr31 pr32 pr35 pr36 pr37
#OBJ:=$(wildcard *.o)
#OBJ=*.o
#VPATH=ish

all:$(BI)

%:%.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@

.PHONY: clean clean_all


clean:
	rm -f $(BI)

clean_all:
	rm -f $(BI) *.c~
