#include <stdlib.h>
#include <stdio.h>
#define SENTINEL 2147483648

typedef struct _ll {
    int value;
    struct _ll *next;
} LinkedList;

int nth(int n, LinkedList *list) {
    if(n == 0)
        return list->value;
    else if(list->next) 
        return nth(n-1, list->next);
    else
        return SENTINEL;
}

LinkedList* makeLinkedList(int value) {
    // malloc a linked list, return the pointer
    LinkedList *list = malloc(sizeof(LinkedList));
    list->value = value;
    list->next = NULL;
    return list;
}

LinkedList* cons(int value, LinkedList *oldHead) {
    LinkedList *newHead = makeLinkedList(value);
    newHead->next = oldHead;
    return newHead;
}

void printLinkedList(LinkedList *ll) {
    while(ll) {
        printf("%d\n", ll->value);
        ll = ll->next;
    }
}

// example program: 
// (define main (lambda (x y) (+ x y)))
int main(int argc, char **argv) {
    // reader: split on () and whitespace to tokenize
    // example program: (car (cdr (quote a b c))) => b
    // generate a data structure in memory to represent the AST
    // how does evaluation work? a pointer to environment and a 
    // pointer to function. call the function with that environment
    // and then return.
    // look for "main" in the environment and execute that first
    // AST structure: environment is a pointer to a hash table
    // 
    LinkedList *ll = makeLinkedList(1);
    LinkedList *newHead = cons(2, ll);
    printLinkedList(newHead);
    printf("Hello world");
}

