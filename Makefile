CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

MyCC: $(OBJS)
	$(CC) -o MyCC $(OBJS) $(LDFLAGS)

$(OBJS): mycc.h

test: MyCC
	./test.sh

clean:
	rm -f MyCC *.o *~ tmp*

.PHONY: test clean