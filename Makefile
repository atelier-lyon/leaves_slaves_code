CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -fsanitize=address -lcriterion -g

BUILD_DIR = ./build
LIB_DIR = $(BUILD_DIR)/lib
LIB_NAME = libproto.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -fsanitize=address -g

TARGET_EXEC := nanoleaf

BUILD_DIR := ./build
SRC_DIRS := ./src
TEST_DIRS := ./test

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
TEST_SRCS := $(shell find $(TEST_DIRS) -name '*.c')

# Prepends BUILD_DIR and appends .o to every src file
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:%=$(BUILD_DIR)/%.o)

all : $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
