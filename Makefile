BIN=gc

SRC=$(wildcard *.c)

CFLAGS = -Wall

.PHONY: clean

$(BIN): clean $(SRC)
	$(CC) -std=gnu99 $(SRC) $(CFLAGS) -o $(BIN)

clean:
	rm -f $(BIN) *~
