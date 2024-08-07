CC = gcc
CFLAGS = -Wall -Wextra -std=c18
CFLAGS += -ffunction-sections -fdata-sections
#CFLAGS += -g # debug
#CFLAGS += -Wno-unused-value -pedantic

# Include header file in all compilation processes
CFLAGS += -include config.h

LDFLAGS = -lncurses
LDFLAGS += -Wl,--print-memory-usage -Wl,--gc-sections

TARGET_EXEC := cli
INSTALL_DIR ?= install
BUILD_DIR := build
SRC_DIRS := src

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
#SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SRCS := $(shell find $(SRC_DIRS) -type f -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
#INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS := include
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@size $(BUILD_DIR)/$(TARGET_EXEC)
	@nm -n --defined-only $(BUILD_DIR)/$(TARGET_EXEC) > $(BUILD_DIR)/symbol.txt
	@nm -nS --defined-only $(BUILD_DIR)/$(TARGET_EXEC) > $(BUILD_DIR)/symbol_size.txt
	@echo "Symbol saved to $(BUILD_DIR)/symbol.txt"
	@echo "Symbol with size saved to $(BUILD_DIR)/symbol_size.txt"

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

install: $(BUILD_DIR)/$(TARGET_EXEC)
	@install -d $(INSTALL_DIR)/bin/
	@install -m 755 $(BUILD_DIR)/$(TARGET_EXEC) $(INSTALL_DIR)/bin/
#@install -d $(INSTALL_DIR)/include/
#@install -m 644 $(INC_DIRS) $(INSTALL_DIR)/include/

.PHONY: clean
clean:
	@rm -rf "$(BUILD_DIR)" "$(INSTALL_DIR)"


# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
