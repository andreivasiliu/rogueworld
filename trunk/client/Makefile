# Generic Makefile for RW.

BIN = rwclient
OBJ = main.o comm_tcp.o rwp_common.o rwp_parse.o rwp_send.o
LIB = -lcurses


all: $(BIN)

$(BIN): $(OBJ)
	gcc -g $^ -o $@ $(LIB)


%.o: %.c common.h
	gcc -c -g -Wall $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

