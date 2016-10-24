CC=gcc -std=c99
CFLAGS = -ggdb3 -W -Wall -Wextra -Werror -O0
LDFLAGS =
LIBS =

default: test 

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

test: btree.o test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f test *.o
