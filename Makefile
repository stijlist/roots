all:
	cc -g -o test.out test.c

test: all
	./test.out
