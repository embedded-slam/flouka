################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: test_flouka

dependents:
	-cd C:/Documents\ and\ Settings/mgalal/workspace/flouka/build && $(MAKE) clean all

# Tool invocations
test_flouka: $(OBJS) $(USER_OBJS) C:/Documents\ and\ Settings/mgalal/workspace/flouka/build/libflouka.a
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L../../flouka/build -o"test_flouka" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) test_flouka
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
C:/Documents\ and\ Settings/mgalal/workspace/flouka/build/libflouka.a:

-include ../makefile.targets
