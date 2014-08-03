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
    ConsCell,
    Nil,
} TypeTag;
typedef struct _value Value;
typedef struct _cons Cons;
typedef union _data Data;

struct _value {
    union _data {
        int number;
        Cons *list;
        char *symbol;
    } data;
    TypeTag tag;
};

struct _cons {
    Value head;
    Value tail;
};

Value symbol(char *buf) {
    return (Value) { (Data) buf, Symbol };
}

Value number(int n) {
    return (Value) { (Data) n, Number };
}

Value nil() {
    return (Value) { (Data) 0, Nil };
}

Value truth() {
    return (Value) { (Data) 1, Truth };
}

Cons* make_cons_cell() {
    Cons *cons_cell = malloc(sizeof(Cons));
    cons_cell->head = nil();
    cons_cell->tail = nil();
    
    return cons_cell;
}

Value cons(Value head, Value tail) {
    Cons *cons = make_cons_cell();
    cons->head = head;
    cons->tail = tail;
    return (Value) { (Data) cons, ConsCell };
}

// returns a value pointer, mutable
Value head(Value v) {
    if (v.tag != ConsCell) {
        printf("Error, calling head on a non-list value.");
        return nil();
    } else {
        return v.data.list->head;
    }
}
    
// return a value pointer, mutable
Value tail(Value v) {
    if (v.tag != ConsCell)
        printf("Error, calling head on a non-list value.");
    else
        return v.data.list->tail;

    return nil();
}

/* TODO: consider whether a pointer return type is correct here */
Value nth(int n, Value current) {
    if (current.tag != ConsCell) 
        printf("Error, calling nth on a non-list value.");
    else if (n == 0)
        return current;
    else
        return nth(n-1, tail(current));
    return nil();
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
    return (c >= 48 && c <= 57);
}

typedef struct _parseresult {
    char *newcursor;
    Value value;
} ParseResult;

ParseResult parsesym(char *cursor) {
    ParseResult result;
    char *symbuf = malloc(MAX_SYMBOL_SIZE * sizeof(char));
    int i;
    for (i = 0; is_alpha(*cursor); cursor++, i++) 
        symbuf[i] = *cursor;
    result = (ParseResult) { cursor, symbol(symbuf) };
    return result;
}

char* next_value_at(char *cursor) {
    while (*cursor == ' ' || *cursor == '\t') 
        cursor++;
    return cursor;
}

int numval(char c) {
    return c - 48;
}

ParseResult parsenum(char *cursor) {
    int acc = 0;
    for (; is_num(*cursor); cursor++) 
        acc = acc * 10 + numval(*cursor);
    return (ParseResult) { cursor, number(acc) };
}

ParseResult parse(char *cursor) {
    ParseResult result, remaining;
    cursor = next_value_at(cursor);
    if (is_open_paren(*cursor)) {
        result = parse(++cursor);
        remaining = parse(result.newcursor);
        return (ParseResult) { remaining.newcursor, cons(result.value, remaining.value) };
    } else if (is_alpha(*cursor)) {
        result = parsesym(cursor);
        remaining = parse(result.newcursor);
        return (ParseResult) { remaining.newcursor, cons(result.value, remaining.value) };
    } else if (is_num(*cursor)) {
        result = parsenum(cursor);
        remaining = parse(next_value_at(result.newcursor));
        return (ParseResult) { remaining.newcursor, cons(result.value, remaining.value) };
    } else if (is_close_paren(*cursor)) {
        return (ParseResult) { ++cursor, nil() };
    } else if (*cursor == 0) {
        return (ParseResult) { cursor, nil() };
    } else {
        printf("Problem reading buffer at:\n %s\n", cursor);
        return (ParseResult) { cursor, nil() };
    }
}

// printList and printValue are mutually recursive; if I switch to using
// header files, I can remove this forward declaration
void printList(Value l); 
void printValue(Value v) {
    switch (v.tag) {
        case Number:
            printf("%d", v.data.number);
            break;
        case Symbol:
            printf("%s", v.data.symbol);
            break;
        case ConsCell:
            printList(v);
            break;
        case Nil:
            printf("nil ");
            break;
    }
}

void printList(Value l) {
    printf("(");
    while(l.tag != Nil) {
        printValue(head(l));
        l = tail(l);
        // TODO: condense conditional
        if (l.tag != Nil) printf(" ");
    }
    printf(")");
}

// example program: 
// (car (cdr (quote a b c))) => b
// example program: 
// (define main (lambda (x y) (+ x y)))
int main() {
    // how does evaluation work? a pointer to environment and a 
    // pointer to function. call the function with that environment
    // and then return.
    // look for "main" in the environment and execute that first
    // AST structure: environment is a pointer to a hash table
    // 
    printf("Hello world\n");
    Value l = cons(number(2), nil());
    
    printf("Testing the print function: ");
    Value newHead = cons(number(1), l);
    printValue(newHead);
    printf("\n");
    printf("Testing the nth function:\n");
    Value first = nth(0, newHead);
    printf("The first element from the head of the linked list is ");
    printValue(first);
    printf("\n");
    Value second = nth(1, newHead);
    printf("The second element from the head of the linked list is ");
    printValue(second);

    printf("\n");
    
    printf("Testing the updated printValue function:\n");
    printValue(newHead);
    printf("\n");

    printf("Testing the parse function with (1 2 3) \n");
    ParseResult result = parse("(1 2 3)");
    printValue(result.value);
    return 0;
}
