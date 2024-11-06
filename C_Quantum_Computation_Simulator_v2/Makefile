# Compiler
CC = gcc
CFLAGS = -Wall -g -I$(LIB_DIR) -lm

# Directories
LIB_DIR = lib
TESTS_DIR = tests

# Library files
LIB_SRC = $(LIB_DIR)/qc_lib.c
LIB_OBJ = $(LIB_DIR)/qc_lib.o
LIB_HEADER = $(LIB_DIR)/qc_lib.h

# Test files
TESTS = $(wildcard $(TESTS_DIR)/*.c)
TEST_EXECUTABLES = $(TESTS:.c=)

# Target for creating the library object
$(LIB_OBJ): $(LIB_SRC) $(LIB_HEADER)
	$(CC) $(CFLAGS) -c $(LIB_SRC) -o $(LIB_OBJ)

# Target for each test executable
$(TEST_EXECUTABLES): $(LIB_OBJ)
	$(CC) $(CFLAGS) $@.c $(LIB_OBJ) -o $@.elf

# Build all tests
.PHONY: all
all: $(TEST_EXECUTABLES)

# Clean up
.PHONY: clean
clean:
	rm -f $(LIB_OBJ) $(TEST_EXECUTABLES)
