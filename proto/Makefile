CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -Werror
LDFLAGS = -fsanitize=address
EXEC = proto

all: $(EXEC)

test: proto.o
	$(CC) -o $@ $^ $(LDFLAGS)

proto.o: proto.c
	$(CC) $(CFLAGS) -o $@ -c $^
