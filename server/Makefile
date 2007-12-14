# Generic Makefile for RW.

BIN = rworld
OBJ = main.o comm_tcp.o rwp_parse.o rwp_send.o players.o rle.o


all: $(BIN)

$(BIN): $(OBJ)
	gcc -g $^ -o $@


%.o: %.c common.h
	gcc -c -g -Wall $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

