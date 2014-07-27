#include <stdlib.h>
#include <stdio.h>
#define MAX_SYMBOL_SIZE 32
typedef enum {
    false,
    true,
} bool;

typedef enum {
    Number,
    Symbol,
    Cons,
} TypeTag;
typedef struct _value Value;
typedef struct _list List;
typedef union _data Data;

struct _value {
    union _data {
        int number;
        List *list;
        char *symbol;
    } data;
    TypeTag tag;
};

struct _list {
    Value value;
    List *next;
};

List* makeList(Value value) {
    // malloc a linked list, return the pointer
    List *list = malloc(sizeof(List));
    list->value = value;
    list->next = NULL;
    return list;
}

/* TODO: consider whether a pointer return type is correct here */
Value* nth(int n, List *list) {
    if(n == 0)
        return &(list->value);
    else if(list->next) 
        return nth(n-1, list->next);
    else
        return NULL;
}

Value symbol(char *buf) {
    return (Value) { (Data) buf, Symbol };
}

Value number(int n) {
    return (Value) { (Data) n, Number };
}
    
bool is_open_paren(char c) {
    return c == 40;
}

bool is_close_paren(char c) {
    return c == 41;
}

bool is_alpha(char c) {
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool is_num(char c) {
    return (c >= 30 && c <= 39);
}

enum ParseError {
    None,
    UnbalancedList,
    MalformedSymbol,
};

typedef struct _parseresult {
    char *newcursor;
    Value value;
    enum ParseError error;
} ParseResult;

ParseResult parsesym(char *cursor) {
    char *symbuf = malloc(MAX_SYMBOL_SIZE * sizeof(char));
    int i;
    for (i = 0; *cursor != ' '; cursor++, i++) 
        symbuf[i] = *cursor;
    while (*cursor == ' ') 
        cursor++;
    return (ParseResult) { cursor, symbol(symbuf) };
}

int numval(char c) {
    return c - 30;
}

ParseResult parsenum(char *cursor) {
    int acc = 0;
    for (; is_num(*cursor); cursor++) 
        acc = acc * 10 + numval(*cursor);
    return (ParseResult) { cursor, number(acc), None };
}

// TODO: test this! forgot to TDD in my excitement
ParseResult parselist(char *buf) {
    List *list = makeList(number(0)); // FIXME: make a nil value?
    ParseResult result; 
    /* consume the initial paren given to parselist, recursively advance 
     * through tokens */
    char *cursor = buf;
    while (!is_close_paren(*cursor)) {
        if (is_open_paren(*cursor)) {
            result = parselist(cursor); 
            cursor = result.newcursor;
            list->value = result.value;
        } else if (is_alpha(*cursor)) {
            result = parsesym(cursor);
            cursor = result.newcursor;
            list->value = result.value;
        } else if (is_num(*cursor)) {
            result = parsenum(cursor);
            cursor = result.newcursor;
            list->value = result.value;
        }
    }
    return result = (ParseResult) { cursor, (Value) { (Data) list, Cons } };
}

/* the cons function adds a new value to the head of a linked list 
 * and returns a pointer to the new head of the list without 
 * mutating or invalidating your old pointer to the head of the list
 * Is this a good idea? I'm not sure. Maybe we want our pointers to
 * the old head of the list to be updated?
 */
List* cons(Value value, List *oldHead) {
    List *newHead = makeList(value);
    newHead->next = oldHead;
    return newHead;
}

void printList(List *l);
void printValue(Value *v) {
    switch (v->tag) {
        case Number:
            printf("%d ", v->data.number);
            break;
        case Symbol:
            printf("%s ", v->data.symbol);
            break;
        case Cons:
            printList(v->data.list);
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
