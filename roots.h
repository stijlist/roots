#include <stdlib.h>
#include <stdio.h>

typedef enum {
    false,
    true,
} bool;

typedef enum {
    Number,
    Symbol,
    ConsCell,
    Lambda,
    Nil,
    Truth
} TypeTag;

typedef struct _value Value;
typedef struct _closure Closure;
typedef struct _cons Cons;
typedef union _data Data;

struct _value {
    union _data {
        int number;
        Cons *list;
        char *symbol;
        Closure *closure;
    } data;
    TypeTag tag;
};

struct _cons {
    Value head;
    Value tail;
};

struct _closure {
    Value symbol;
    Value body;
    Value env;
};

typedef struct _parseresult {
    char *newcursor;
    Value value;
} ParseResult;

Value eval_empty(Value arg);
Value eval(Value arg, Value env);
ParseResult parse(char *cursor);
Value read(char *string);
void annotate(Value l, char* annotation); 
void printList(Value l); 
void printValue(Value v);
