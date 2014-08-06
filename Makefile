all: test main
	cc roots.o test.o -o test.out 

debug:
	cc -g -c test.c
	cc -g -c roots.c
	cc roots.o test.o -o debug.out 
	lldb ./debug.out

clean:
	rm -rf *.o *.dSYM *.out

test:
	cc -c test.c

main: 
	cc -c roots.c

run: all
	./test.out
