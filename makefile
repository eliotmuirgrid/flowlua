TARGET    := flow.com
BUILD_DIR := build

TOOL_DIR := $(HOME)/cosmo/bin/

CC  := $(TOOL_DIR)cosmocc
CXX := $(TOOL_DIR)cosmoc++

# Find C and C++ source files in immediate subdirectories.
DIRS    := $(wildcard ./*/)
SEARCH  := $(foreach dir,$(DIRS),$(dir)*.c $(dir)*.cpp)
SOURCES := $(wildcard $(SEARCH))

# Convert:
#   COL/COLstring.cpp     -> build/COL/example.o
OBJECTS := $(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SOURCES)))
OBJECTS += $(patsubst ./%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SOURCES)))

# Add any explicitly supplied object files.
OBJECTS += $(OBJECTS_EXTRA)

# -MMD generates a .d file beside each object file.
DEPENDS := $(OBJECTS:.o=.d)

PCFLAGS   := -Wno-pointer-to-int-cast -MMD -MP -I.
PCPPFLAGS := -MMD -MP -I.

.PHONY: all clean show depends

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(PCFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(PCPPFLAGS) -c -o $@ $<

depends:
	@echo DEPENDS = $(DEPENDS)

show:
	@echo "Making: $(TARGET)"
	@echo DIRS    = $(DIRS)
	@echo CC      = $(CC)
	@echo SEARCH  = $(SEARCH)
	@echo SOURCES = $(SOURCES)
	@echo OBJECTS = $(OBJECTS)
	@echo DEPENDS = $(DEPENDS)

clean::
	$(RM) -r $(BUILD_DIR) *.elf *.dbg

-include $(DEPENDS)

