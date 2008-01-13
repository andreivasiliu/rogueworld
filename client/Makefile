# Generic Makefile for RW.

BIN += rwclient
OBJ += main.o comm_udp.o rwp_common.o rwp_parse.o rwp_send.o update.o
OBJ += events.o objects.o utils.o
LIB += -lcurses $(WINLIB)

CFLAGS = -g -Wall
LFLAGS = $(LIB)


all: $(BIN)

# Further dependencies.
rwp_common.o: rwp_common.h
rwp_parse.o: rwp_common.h
rwp_send.o: rwp_common.h


$(BIN): $(OBJ)
	gcc -o $@ $^ $(LFLAGS)

%.o: %.c common.h
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(BIN) $(OBJ)

