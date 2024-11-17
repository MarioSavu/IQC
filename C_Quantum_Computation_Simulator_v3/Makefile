CC = gcc
CFLAGS = -Wall -g -Ilib/include -lm
BUILD_DIR = build
SRC_DIR = lib/src
TEST_DIR = tests
EXAMPLE_DIR = examples

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
EXAMPLE_FILES = $(wildcard $(EXAMPLE_DIR)/*.c)

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_EXECUTABLES = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%, $(TEST_FILES))
EXAMPLE_EXECUTABLES = $(patsubst $(EXAMPLE_DIR)/%.c, $(BUILD_DIR)/%, $(EXAMPLE_FILES))

# Default target to build everything
all: $(BUILD_DIR)/libqc.a $(TEST_EXECUTABLES) $(EXAMPLE_EXECUTABLES)

# Create static library
$(BUILD_DIR)/libqc.a: $(OBJ_FILES)
	ar rcs $@ $^

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile tests and link against the library
$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(BUILD_DIR)/libqc.a
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -lqc -o $@

# Compile examples and link against the library
$(BUILD_DIR)/%: $(EXAMPLE_DIR)/%.c $(BUILD_DIR)/libqc.a
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -lqc -o $@

# Run all tests
test: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test..."; \
		./$$test; \
	done

# Clean all compiled files
clean:
	rm -rf $(BUILD_DIR)/*
