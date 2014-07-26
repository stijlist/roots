#include <stdlib.h>
#include <stdio.h>

typedef union {
    int integer;
    char *symbol;
} Data;

typedef enum {
    Number,
    Symbol,
} TypeTag;

typedef struct {
    union {
        int number;
        char *symbol;
    } data;
    TypeTag tag;
} Value;

typedef struct _list {
    Value value;
    struct _list *next;
} List;

/* TODO: consider whether a pointer return type is correct here */
Value* nth(int n, List *list) {
    if(n == 0)
        return &(list->value);
    else if(list->next) 
        return nth(n-1, list->next);
    else
        return NULL;
}

Value number(int n) {
    return (Value) { n, Number };
}
    

List* parseList() {
    return NULL;
}

List* makeList(Value value) {
    // malloc a linked list, return the pointer
    List *list = malloc(sizeof(List));
    list->value = value;
    list->next = NULL;
    return list;
}

/* the cons function adds a new value to the head of a linked list 
 * and returns a pointer to the new head of the list without 
 * mutating or invalidating your old pointer to the head of the list
 */
List* cons(Value value, List *oldHead) {
    List *newHead = makeList(value);
    newHead->next = oldHead;
    return newHead;
}

void printValue(Value *v) {
    switch (v->tag) {
        case Number:
            printf("%d ", v->data.number);
            break;
        case Symbol:
            printf("%s ", v->data.symbol);
            break;
    }
}
void printList(List *l) {
    while(l) {
        printValue(&(l->value));
        l = l->next;
    }
    printf("\n");
}

// example program: 
// (car (cdr (quote a b c))) => b
// example program: 
// (define main (lambda (x y) (+ x y)))
int main(int argc, char **argv) {
    // reader: split on () and whitespace to tokenize
    // generate a data structure in memory to represent the AST
    // how does evaluation work? a pointer to environment and a 
    // pointer to function. call the function with that environment
    // and then return.
    // look for "main" in the environment and execute that first
    // AST structure: environment is a pointer to a hash table
    // 
    printf("Hello world\n");
    List *l = makeList(number(2));
    
    printf("Testing the print function: ");
    List *newHead = cons(number(1), l);
    printList(newHead);
    printf("Testing the nth function:\n");
    Value *first = nth(0, newHead);
    printf("The first element from the head of the linked list is ");
    printValue(first);
    printf("\n");
    Value *second = nth(1, newHead);
    printf("The second element from the head of the linked list is ");
    printValue(second);
    return 0;
}
