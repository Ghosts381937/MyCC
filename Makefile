CFLAGS=-std=c12 -g -static

MyCC: main.c

test: MyCC
	./test.sh

clean:
	rm -f MyCC *.o *~ tmp*

.PHONY: test clean
