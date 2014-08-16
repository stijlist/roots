#include "roots.h"

void eval_and_print(char *list, Table initial_env) {
    printf("eval %s\n", list); 
    printf("Output: \n");
    printValue(eval(parse(list).value, initial_env));
    printf("\n");
    printf("\n");
}

int main() {
    // how does evaluation work? a pointer to environment and a 
    // pointer to function. call the function with that environment
    // and then return.
    // look for "main" in the environment and execute that first
    // AST structure: environment is a pointer to a hash table
    // 
    printf("Hello world.\n\n");
    Table env = make_env();

    eval_and_print("(cons 1 2)", env); // oops
    eval_and_print("(car (1 2))", env);
    eval_and_print("(cdr (1 2))", env);
    eval_and_print("(quote (1 2))", env);
    eval_and_print("(eq foo foo)", env);
    eval_and_print("(atom foo)", env);
    eval_and_print("(atom (1 2))", env);
    eval_and_print("(if t 1 0)", env);
    eval_and_print("(if (atom ()) (cons 1 2) 0)", env);
    eval_and_print("(if (atom (1 2)) (cons 1 2) 0)", env);

    eval_and_print("(let (n 1) n)", env);
    eval_and_print("(let (n 1) (cons n n))", env);
    // eval_and_print("(let (n 1) (cons n (2 3))", env); unbalanced parens
    // segfault????
    return 0;
}
