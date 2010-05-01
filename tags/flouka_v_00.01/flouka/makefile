CC=gcc
AR=ar
RM= rm -rf
CFLAGS= -DDEBUG -O0 -g3 -pedantic -pedantic-errors -Wall -Werror -I. -c
SOURCES=flouka.c 
OBJECTS=$(SOURCES:.c=.o)
LIBRARY=libflouka.a

all: $(SOURCES) $(LIBRARY)
	
$(LIBRARY): $(OBJECTS) 
	$(AR) -r "$(LIBRARY)" $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o *.a *.d