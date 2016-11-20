CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=  Misc.cc  Error.cc  String.cc  Terminal.cc  mossim.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=mossim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o mossim
