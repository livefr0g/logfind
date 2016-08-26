CFLAGS=-Wall -g

all: logfind tests

#logfind:

tests:
	sh tests.sh

clean:
	rm logfind
	rm -f *.o
	rm -rf *.dSYM