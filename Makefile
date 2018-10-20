src = $(wildcard *.c)
src_test = $(wildcard tests/*.c)
obj = $(src:.c=.o)
test = $(src_test:.c=)

CC = gcc
CFLAGS = -O3 -std=c11 -Wall -Wextra -pedantic
DBGFLAGS = -g -std=c11 -Wall -Wextra -pedantic
TESTFLAGS = $(shell pkg-config --cflags --libs check)
LDFLAGS = 


.PHONY: all debug test clean


all: datastructures


debug: CFLAGS := ${DBGFLAGS}
debug: LDFLAGS += -g
debug: datastructures


datastructures: ${obj}
	${CC} ${LDFLAGS} -o $@ $^


tests/test_%: tests/test_%.c %.o
	${CC} ${DBGFLAGS} -o $@ $^ ${TESTFLAGS}


tests/test_hashmap: tests/test_hashmap.c hashmap.o hash.o
	${CC} ${DBGFLAGS} -o $@ $^ ${TESTFLAGS}


test: debug ${test}
	$(foreach t,$(test),$(t))


clean:
	rm -vf ${obj} ${test} datastructures
