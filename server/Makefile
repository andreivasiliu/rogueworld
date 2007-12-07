
all: main.o comm.o


%.o: %.c common.h
	gcc -c -g -Wall $< -o $@

clean:
	rm -f *.o

