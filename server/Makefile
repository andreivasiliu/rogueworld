# Generic Makefile for RW.

BIN = rworld
OBJ = main.o comm_tcp.o rwp_common.o rwp_parse.o rwp_send.o players.o


all: $(BIN)


# Further dependencies.
rwp_common.o: rwp_common.h
rwp_parse.o: rwp_common.h
rwp_send.o: rwp_common.h


$(BIN): $(OBJ)
	gcc -g $^ -o $@


%.o: %.c common.h
	gcc -c -g -Wall $< -o $@

clean:
	rm -f $(BIN) $(OBJ)

