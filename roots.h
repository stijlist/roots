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

typedef struct _table {
    Cons key[256]; // 256 pairs in my symbol table
} Table;

Table make_env();

bool is_empty(Value v);
Value car(Value arg);
Value eval(Value arg, Table env);
ParseResult parse(char *cursor);
void printList(Value l); 
void printValue(Value v);
