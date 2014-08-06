#include "roots.c"

void eval_and_print(char *list) {
    printf("eval %s\n", list); 
    printf("Output: \n");
    printValue(eval(parse(list).value));
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

    eval_and_print("(cons 1 2)");
    eval_and_print("(car (1 2))");
    eval_and_print("(cdr (1 2))");
    eval_and_print("(quote (1 2))");
    eval_and_print("(cons (1) 2)");
    eval_and_print("(quote (1) 2)");

    return 0;
}
