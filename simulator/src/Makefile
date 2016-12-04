CC=g++
CFLAGS=-c -Wall -std=c++11 
LDFLAGS=
SOURCES=  Misc.cc  Error.cc  String.cc CommandLine.cc Terminal.cc 6502.cc mossim.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=mossim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -std=c++11 $(LDFLAGS) -Wl,--start-group $(OBJECTS) -Wl,--end-group -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o mossim
