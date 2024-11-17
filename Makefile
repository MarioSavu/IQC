# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Ilib/include -lm

# Directories
SRC_DIR = lib/src
INCLUDE_DIR = lib/include
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
EXAMPLES_DIR = examples
TESTS_DIR = tests

# Library
LIB = $(LIB_DIR)/libqc.a

# Source files and objects
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Examples and tests
EXAMPLES = $(patsubst $(EXAMPLES_DIR)/%.c, $(EXAMPLES_DIR)/%.elf, $(wildcard $(EXAMPLES_DIR)/*.c))
TESTS = $(patsubst $(TESTS_DIR)/%.c, $(TESTS_DIR)/%.elf, $(wildcard $(TESTS_DIR)/*.c))

.PHONY: all clean test

# Default target: build everything
all: $(LIB) $(EXAMPLES) $(TESTS)

# Build the static library
$(LIB): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# Compile object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build example executables
$(EXAMPLES_DIR)/%.elf: $(EXAMPLES_DIR)/%.c $(LIB)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -lqc -o $@

# Build test executables
$(TESTS_DIR)/%.elf: $(TESTS_DIR)/%.c $(LIB)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -lqc -o $@

# Run all test executables
test: $(TESTS)
	@for test in $(TESTS); do \
		echo "Running $$test"; \
		./$$test; \
	done

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(EXAMPLES_DIR)/*.elf $(TESTS_DIR)/*.elf
