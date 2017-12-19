# project name (generate executable with this name)
TARGET   = new_project.out

CC       = gcc
# compiling flags here (try "-std=c99")
CFLAGS   = -Wall -I.

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -I. -lm

# change these to proper directories where each file should be
SRCDIR   = src
# OTHERDIR = other_source_dir
# INCDIR   = inc
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c) # \
#			$(wildcard $(OTHERDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)  # If directory BINDIR does not exist, create it (only works in UNIX environment and it is useful when cloning with git)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)  # Check for changes in both .c and .h files. In case, recompile following dependencies
	@mkdir -p $(@D)  # If directory OBJDIR does not exist, create it (only works in UNIX environment and it is useful when cloning with git)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"
	@echo "$(CC) $(CFLAGS) -c $< -o $@"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
