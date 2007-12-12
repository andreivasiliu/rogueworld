# Generic Makefile for RW.

BIN = rworld
OBJ = main.o comm.o protocol.o players.o


all: $(BIN)

$(BIN): $(OBJ)
	gcc -g $^ -o $@


%.o: %.c common.h
	gcc -c -g -Wall $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

