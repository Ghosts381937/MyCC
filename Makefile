CFLAGS=-std=c11 -g -static

MyCC: main.o
	$(CC) -o $@ $? $(LDFLAGS)
test: MyCC
	./test.sh

clean:
	rm -f MyCC *.o *~ tmp*

.PHONY: test clean
