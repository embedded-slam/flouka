CC=gcc
AR=ar
RM= rm -rf
CFLAGS= -DDEBUG -O0 -g3 -pedantic -pedantic-errors -Wall -Werror -I../flouka -c
LDFLAGS= -L../flouka -lflouka
SOURCES=main.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test_flouka

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS) 

clean:
	$(RM) *.o *.a *.d $(EXECUTABLE)