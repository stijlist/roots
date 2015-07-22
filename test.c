#include "roots.h"

void eval_and_print(char *list) {
    printf("%s\n", list); 
    printf("=> ");
    printValue(eval_empty(read(list).value));
    printf("\n");
}

int main() {
    printf("Hello world.\n\n");

    eval_and_print("(cons 1 2)");
    eval_and_print("(cons 1 (quote (2 3)))");
    eval_and_print("(cons (quote (1 2)) 3)");
    eval_and_print("(head (quote (1 2)))");
    eval_and_print("(tail (quote (1 2)))");
    eval_and_print("(quote (1 2))");
    eval_and_print("(eq (quote foo) (quote foo))");
    eval_and_print("(eq (quote foo) (quote bar))");
    eval_and_print("(eq 1 1)");
    eval_and_print("(eq (head (quote (1))) 1)");

    eval_and_print("(head (quote (foo bar)))");

    eval_and_print("(eq 1 (head (quote (1 2))))");
    eval_and_print("(eq (quote foo) (head (cons (quote foo) (quote bar))))");

    eval_and_print("(atom (quote foo))");
    eval_and_print("(atom (quote (1 2)))");

    eval_and_print("(if t 1 0)");
    eval_and_print("(if (atom ()) (cons 1 2) 0)");
    eval_and_print("(if (atom (quote (1 2))) (cons 1 2) 0)");

    eval_and_print("(let (n 1) n)");
    eval_and_print("(let (n 1) (cons n 1))");
    eval_and_print("(let (n 1) (cons n n))");
    eval_and_print("(let (x (cons 1 2)) x)");
    eval_and_print("(let (x (cons 1 2)) (cons x 3))");
    eval_and_print("(let (x 1) (let (y 2) (cons x y)))");

    eval_and_print("((lambda x x) 2)");
    eval_and_print("((lambda x (cons 1 x)) 2)");

    eval_and_print("((lambda x ((lambda y (cons x y)) 2)) 1)");
    eval_and_print("((lambda x ((lambda x (cons x x)) 2)) 1)");
    return 0;
}
