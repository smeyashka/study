LDFLAGS=-lglut -lGL -lGLU -lm
CFLAGS=-g -Wall
CC=gcc

BI=2_6 2_6_1 2_6_2 2_6_3 u2_1 u2_1_1 u2_6 u2_16 3_5 3_8

all:$(BI)

%:%.c
	$(CC) $< $(CFLAGS) $(LDFLAGS) -o $@


.PHONY: clean clean_all

clean:
	rm -f $(BI)

clean_all:
	rm -f $(BI) *.c~
