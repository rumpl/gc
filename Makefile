BIN=gc

SRC=$(wildcard *.c)

CFLAGS = -Wall -pedantic -pedantic-errors

.PHONY: clean

$(BIN): clean $(SRC)
	$(CC) -std=c11 $(SRC) $(CFLAGS) -o $(BIN)

clean:
	rm -f $(BIN) *~
