# Generic Makefile for RW.

BIN = rwclient
OBJ = main.o comm_tcp.o rwp_common.o rwp_parse.o rwp_send.o update.o events.o
LIB = -lcurses


all: $(BIN)


# Further dependencies.
rwp_common.o: rwp_common.h
rwp_parse.o: rwp_common.h
rwp_send.o: rwp_common.h


$(BIN): $(OBJ)
	gcc -g $^ -o $@ $(LIB)


%.o: %.c common.h
	gcc -c -g -Wall $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

