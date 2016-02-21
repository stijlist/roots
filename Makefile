main:
	cc -c roots.c utils.c repl.c
	cc roots.o utils.o repl.o -o repl.out

test: main
	cc -c test.c 
	cc utils.o roots.o test.o -o test.out
	./test.out

run: main
	./repl.out

debug:
	cc -g -c test.c
	cc -g -c roots.c
	cc roots.o test.o -o debug.out 
	lldb ./debug.out

clean:
	rm -rf *.o *.dSYM *.out
