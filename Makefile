main:
	cc -g roots.c repl.c -o roots

test:
	cc -g roots.c test.c -o test.out
	./test.out

run: main
	./roots

debug-repl: main
	lldb ./roots

debug:
	cc -g -c test.c
	cc -g -c roots.c
	cc roots.o test.o -o debug.out 
	lldb ./debug.out

clean:
	rm -rf *.o *.dSYM *.out
