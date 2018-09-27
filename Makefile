src = $(wildcard *.c)
src_test = $(wildcard tests/*.c)
obj = $(src:.c=.o)
test = $(src_test:.c=)

CC = gcc
CFLAGS = -O3 -Wall -Wextra -pedantic -fdiagnostics-color=always
LDFLAGS = 


.PHONY: clean test


datastructures: ${obj}
	${CC} ${LDFLAGS} -o $@ $^


tests/test_%: tests/test_%.c %.o
	${CC} ${CFLAGS} -lcheck -o $@ $^


test: ${test}
	$(foreach t,$(test),$(t))


clean:
	rm -vf ${obj} ${test} datastructures
