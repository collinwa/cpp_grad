CC  = g++
CXX = g++ 

INCLUDES = -DPRINTGRAD
#INCLUDES = 
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES) -std=c++11

LDFLAGS = -g
LDLIBS  =

executables = run_autograd
objects = Value.o run_autograd.o Matrix.o

.PHONY: default
default: $(executables)

$(executables): $(objects)

$(objects): Value.h Matrix.h

.PHONY: clean
clean:
	rm -f *~ a.out core $(objects) $(executables)

.PHONY: all
all: clean default
