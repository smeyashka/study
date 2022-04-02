CXXFLAGS= -Wall -O2
LDFLAGS=-lm `sdl-config --libs` -lSDL -lSDL_image -lSDL_ttf -lSDL_net
CXX=g++

#SRC := $(shell find $(SRCDIR) -name "*.cc")

OBJDIR=.o
OBJ=$(addprefix $(OBJDIR)/, image.o chip.o  window.o field.o remotemodel.o client.o server.o main.o)

BI=reversi


all: reversi

.PHONY: make_dir

make_dir:
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)


reversi:make_dir $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o reversi



# %.o:%.cc #%.h
# 	$(OBJDIR)/%.o: %.cc %.h

#%.o:%.cc #%.h

$(OBJDIR)/main.o: main.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@



.PHONY: clean_old clean

clean:
	rm -rf .o $(BI)

clean_old:
	rm -f *~

clean_all: clean clean_old
