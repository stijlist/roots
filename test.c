#include "roots.c"

int main() {
    // how does evaluation work? a pointer to environment and a 
    // pointer to function. call the function with that environment
    // and then return.
    // look for "main" in the environment and execute that first
    // AST structure: environment is a pointer to a hash table
    // 
    printf("Hello world\n");

    ParseResult result;
    printf("Testing the parse function with (1 (2) ((3 4) 5))\n");
    result = parse("(1 (2) ((3 4) 5))");
    printValue(result.value);
    printf("\n");

    printf("Testing the parse function with (symbol 1 (2) ((3 4) 5)) \n");
    result = parse("(symbol 1 (2) ((3 4) 5))");
    printValue(result.value);
    printf("\n");

//    printf("Testing the streq function with 'hello' and 'hello'.\n");
//    if (streq("hello", "hello"))
//        printf("True\n");
//    else
//        printf("False\n");

    result = parse("(quote (1 2 3))");
    printf("Testing eval function with (quote (1 2 3))\n");
    printf("Input: \n");
    printValue(result.value);
    printf("\nOutput: \n");
    printValue(eval(result.value));
    printf("\n");

    result = parse("(car (1 2 3))");
    printf("Testing eval function with (car (1 2 3))\n");
    printf("Input: \n");
    printValue(result.value);
    printf("\nOutput: \n");
    printValue(eval(result.value));
    printf("\n");

    result = parse("(1 2 3)");
    printf("Testing car function with (1 2 3)\n");
    printf("Input: \n");
    printValue(result.value);
    printf("\n");
    printf("Output: \n");
    printValue(car(result.value));
    printf("\n");

    result = parse("(1 2 3)");
    printf("Testing quote function with (1 2 3)\n");
    printf("Input: \n");
    printValue(result.value);
    printf("\nOutput: \n");
    printValue(quote(result.value));
    printf("\n");

    return 0;
}
