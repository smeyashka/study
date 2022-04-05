CXXFLAGS= -c  -Wall #-O2
#LDFLAGS=-L. -lcalc
LDFLAGS=
CC=g++
CALC=arithm.cpp lexer.cpp table.cpp parser.cpp
HEAD=arithm.h lexer.h error.h table.h parser.h parser_impl.h driver.h
OBJDIR=.o
OBJ=$(addprefix $(OBJDIR)/, arithm.o lexer.o table.o parser.o driver.o)


all:calc

.PHONY: make_dir

make_dir:
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)


calc_st: make_dir stat $(OBJDIR)/main.o
	$(CC) $(OBJDIR)/main.o libcalc.a -o calc_st

# calc_din:main.o
#	$(CC) $(LDFLAGS) main.o -o calc_din

calc_din: make_dir din $(OBJDIR)/main.o
	$(CC) $(OBJDIR)/main.o libcalc.so -o calc_din


stat: ob
	ar -cr libcalc.a $(OBJ)
	ranlib libcalc.a

# arithm.o:arithm.cc arithm.h
# 	$(CC) -c $^ -o $@

ob:$(OBJ)


$(OBJDIR)/%.o: %.cpp %.h
	$(CC) $(CXXFLAGS) $< -o $@

$(OBJDIR)/main.o:main.cpp
	$(CC) -c main.cpp -o $(OBJDIR)/main.o


din:$(OBJ)
	$(CC) -fPIC -shared -o libcalc.so $(OBJ)



#----------------
calc: make_dir  $(OBJDIR)/main.o $(OBJ)
	$(CC) $(LDFLAGS) --static -o calc  $(OBJDIR)/main.o $(OBJ)
#-----------------------------------


clean:
	rm -f $(OBJ) $(OBJDIR)/main.o calc

clean_stat:
	rm -f libcalc.a $(OBJ) $(OBJDIR)/main.o calc_st

clean_din:
	rm -f $(OBJ) $(OBJDIR)/main.o libcalc.so calc_din
