CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors -std=c99
LDFLAGS = -lm
SRC = symnmf.c
EXECUTABLE = symnmf

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean run
