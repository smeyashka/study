LDFLAGS=-lglut -lGL -lGLU
CFLAGS=-g -c
CC=gcc
BI=2_6 2_6_1 2_6_2 2_6_3 u2_1 u2_1_1 u2_6 u2_16 3_5 3_8

all:$(BI)

2_6: $(LDFLAGS) 2_6.o
2_6_1: $(LDFLAGS) 2_6_1.o
2_6_2: $(LDFLAGS) 2_6_2.o
2_6_3: $(LDFLAGS) 2_6_3.o
u2_1: $(LDFLAGS) u2_1.o
u2_1_1: $(LDFLAGS) u2_1_1.o
u2_6: $(LDFLAGS) u2_6.o
u2_6:$(LDFLAGS) -lm u2_6.o 
u2_16: $(LDFLAGS) -lm u2_16.o
3_5: $(LDFLAGS) 3_5.o
3_8: $(LDFLAGS) 3_8.o

clean:
	rm -f *.o $(BI)
