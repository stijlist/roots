#include "roots.h"

void eval_and_print(char *list, Table initial_env) {
    printf("%s\n", list); 
    printf("=> ");
    printValue(eval(parse(list).value, initial_env));
    printf("\n");
}

int main() {
    printf("Hello world.\n\n");
    Table env = make_env();

    eval_and_print("(cons 1 2)", env);
    eval_and_print("(cons 1 (quote (2 3)))", env);
    eval_and_print("(cons (quote (1 2)) 3)", env);
    eval_and_print("(car (quote (1 2)))", env);
    eval_and_print("(cdr (quote (1 2)))", env);
    eval_and_print("(quote (1 2))", env);
    eval_and_print("(eq (quote foo) (quote foo))", env);
    eval_and_print("(eq (quote foo) (quote bar))", env);
    eval_and_print("(eq 1 1)", env);
    eval_and_print("(eq (car (quote (1))) 1)", env);

    eval_and_print("(car (quote (foo bar)))", env);

    eval_and_print("(eq 1 (car (quote (1 2))))", env);
    eval_and_print("(eq (quote foo) (car (cons (quote foo) (quote bar))))", env);

    eval_and_print("(atom foo)", env);
    eval_and_print("(atom (quote (1 2)))", env);

    eval_and_print("(if t 1 0)", env);
    eval_and_print("(if (atom ()) (cons 1 2) 0)", env);
    eval_and_print("(if (atom (quote (1 2))) (cons 1 2) 0)", env);

    eval_and_print("(let (n 1) n)", env);
    eval_and_print("(let (n 1) (cons n 1))", env);
    eval_and_print("(let (n 1) (cons n n))", env);
    eval_and_print("(let (x (cons 1 2)) x)", env);
    eval_and_print("(let (x (cons 1 2)) (cons x 3))", env);
    eval_and_print("(let (x 1) (let (y 2) (cons x y)))", env);

    eval_and_print("((lambda x x) 2)", env);
    eval_and_print("((lambda x (cons 1 x)) 2)", env);
  
    eval_and_print("((lambda x ((lambda y (cons x y)) 2)) 1)", env);
    return 0;
}
