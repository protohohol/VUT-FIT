CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpcap
SOURCES=ipk-sniffer.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ipk-sniffer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)