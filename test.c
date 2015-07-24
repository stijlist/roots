#include "roots.h"

void read_eval_print(char *list) {
    printf("%s\n", list); 
    printf("=> ");
    printValue(eval_empty(read(list)));
    printf("\n");
}

int main() {
    printf("Hello world.\n\n");

    read_eval_print("(cons 1 2)");
    read_eval_print("(cons 1 (quote (2 3)))");
    read_eval_print("(cons (quote (1 2)) 3)");
    read_eval_print("(head (quote (1 2)))");
    read_eval_print("(tail (quote (1 2)))");
    read_eval_print("(quote (1 2))");
    read_eval_print("(eq (quote foo) (quote foo))");
    read_eval_print("(eq (quote foo) (quote bar))");
    read_eval_print("(eq 1 1)");
    read_eval_print("(eq (head (quote (1))) 1)");

    read_eval_print("(head (quote (foo bar)))");

    read_eval_print("(eq 1 (head (quote (1 2))))");
    read_eval_print("(eq (quote foo) (head (cons (quote foo) (quote bar))))");

    read_eval_print("(atom (quote foo))");
    read_eval_print("(atom (quote (1 2)))");

    read_eval_print("(if t 1 0)");
    read_eval_print("(if (atom ()) (cons 1 2) 0)");
    read_eval_print("(if (atom (quote (1 2))) (cons 1 2) 0)");

    read_eval_print("(let (n 1) n)");
    read_eval_print("(let (n 1) (cons n 1))");
    read_eval_print("(let (n 1) (cons n n))");
    read_eval_print("(let (x (cons 1 2)) x)");
    read_eval_print("(let (x (cons 1 2)) (cons x 3))");
    read_eval_print("(let (x 1) (let (y 2) (cons x y)))");

    read_eval_print("((lambda x x) 2)");
    read_eval_print("((lambda x (cons 1 x)) 2)");

    read_eval_print("((lambda x ((lambda y (cons x y)) 2)) 1)");
    read_eval_print("((lambda x ((lambda x (cons x x)) 2)) 1)");
    
    read_eval_print("(let (f (lambda x x)) (f 1))");
    read_eval_print("(lambda x x)");
    return 0;
}
