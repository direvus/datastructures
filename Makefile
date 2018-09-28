src = $(wildcard *.c)
src_test = $(wildcard tests/*.c)
obj = $(src:.c=.o)
test = $(src_test:.c=)

CC = gcc
CFLAGS = -O3 -std=c11 -Wall -Wextra -pedantic
TESTFLAGS = $(shell pkg-config --cflags --libs check)
LDFLAGS = 


.PHONY: clean test


datastructures: ${obj}
	${CC} ${LDFLAGS} -o $@ $^


tests/test_%: tests/test_%.c %.o
	${CC} ${CFLAGS} ${TESTFLAGS} -o $@ $^


test: ${test}
	$(foreach t,$(test),$(t))


clean:
	rm -vf ${obj} ${test} datastructures
