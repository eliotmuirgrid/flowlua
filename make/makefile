TOOL_DIR:=$(HOME)/flowtools/cosmopolitan/bin/

# Wildcard matching to figure out where sources are based on the DIRS variable
OBJEXT  := .o
LIBEXT  := a

# Safely expand search paths
DIRS    := $(wildcard ../*/)
SEARCH  := $(foreach dir, $(DIRS), $(dir)*.c $(dir)*.cpp) 
SOURCES := $(wildcard $(SEARCH))

# Use patsubst instead of subst to prevent corrupting folder names containing ".c"
OBJECTS := $(patsubst %.cpp, %$(OBJEXT), $(SOURCES))
OBJECTS := $(patsubst %.cpp, %$(OBJEXT), $(SOURCES))
OBJECTS := $(patsubst %.c, %$(OBJEXT), $(OBJECTS))
DEPENDS := $(patsubst %$(OBJEXT), %.o.d, $(OBJECTS))

OBJECTS := $(OBJECTS) $(OBJECTS_EXTRA)

# If a target is not defined, default to test.com
TARGET  ?= test.com

# Define our clean list
RMLIST  := $(foreach dir, $(DIRS), $(dir)*.o $(dir)*.d) *.d *.o *~ *.elf *.dbg 

.PHONY: all info clean

all: $(TARGET)

CC     := $(TOOL_DIR)cosmocc
CXX    := $(TOOL_DIR)cosmoc++

PCFLAGS := -Wno-pointer-to-int-cast -MMD -MP -I../ 
PCPPFLAGS := -MMD -MP -I../ 

$(TARGET): $(OBJECTS) 
	$(CXX) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(PCFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(PCPPFLAGS) -c -o $@ $<

depends:
	@echo DEPENDS = $(DEPENDS)

show:
	@echo "Making: " $(TARGET)
	@echo CC      = $(CC)
	@echo SEARCH  = $(SEARCH)
	@echo SOURCES = $(SOURCES)
	@echo OBJECTS = $(OBJECTS)
	@echo DEPENDS = $(DEPENDS)

# Use double colon rule for clean so regular makefiles can perform additional cleaning. 
clean::
	-$(RM) $(RMLIST)

-include $(DEPENDS)

