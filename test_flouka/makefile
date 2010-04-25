CC=gcc
CFLAGS=-c -Wall
LDFLAGS= -L../flouka/lib -lflouka
SOURCES=main.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test_flouka

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

