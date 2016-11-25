CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SOURCES=  Misc.cpp  Error.cpp  String.cpp  Terminal.cpp  mossim.cpp cpu.cpp
INSTRUCTIONS= cpu.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mossim

all: $(EXECUTABLE)

mossim: 
	$(CC) $(LDFLAGS) Misc.o Error.o String.o Terminal.o mossim.o cpu.o -o mossim

#.cpp.o: 
#	cd insrtuctions && $(CC) $(CFLAGS) $< -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o mossim
