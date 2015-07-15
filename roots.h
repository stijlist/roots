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

typedef struct _parseresult {
    char *newcursor;
    Value value;
} ParseResult;

bool is_empty(Value v);
Value car(Value arg);
Value eval_empty(Value arg);
Value eval(Value arg, Value env);
ParseResult parse(char *cursor);
void printList(Value l); 
void printValue(Value v);
