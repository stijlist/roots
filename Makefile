main:
	cc roots.c repl.c -o roots

test:
	cc roots.c test.c -o test.out
	./test.out

run: main
	./roots

debug:
	cc -g -c test.c
	cc -g -c roots.c
	cc roots.o test.o -o debug.out 
	lldb ./debug.out

clean:
	rm -rf *.o *.dSYM *.out
