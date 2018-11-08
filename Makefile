
CC=g++
CXX=g++
CXXFLAGS = -std=c++17 -Ofast -march=native -fopenmp

src = $(wildcard src/*.cpp)\
	  $(wildcard src/*/*.cpp)
	    
obj = $(src:.cpp=.o) 

LDFLAGS = -lGL -lglut -lpng -lz -lm

myprog: $(obj)
	g++ -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) myprog