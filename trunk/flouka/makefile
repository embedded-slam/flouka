CC=gcc
CFLAGS=-c -Wall
LDFLAGS= -L../flouka -lflouka
SOURCES=main.c 
OBJECTS=$(SOURCES:.c=.o)

# All Target
all: libflouka.a

# Tool invocations
libflouka.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r "libflouka.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ARCHIVES) libflouka.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

