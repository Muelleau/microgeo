
CC=g++
CXX=g++
CXXFLAGS = -std=c++17 -Ofast -march=native -fopenmp

src = $(wildcard src/*.cpp)\
	  $(wildcard src/*/*.cpp)
	    
obj = $(src:.cpp=.o) 


LDFLAGS = -lGL -lglut -lpng -lz -lm

microgeo: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)


.PHONY: clean
clean:
	rm -f $(obj) myprog


INSTALL_PREFIX = ~

.PHONY: install
install: microgeo
	mkdir -p $(DESTDIR)$(INSTALL_PREFIX)/bin
	cp $< $(DESTDIR)$(INSTALL_PREFIX)/bin/microgeo

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(INSTALL_PREFIX)/bin/microgeo

