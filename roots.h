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

Value eval(Value arg, Value env);
Value read(char *string);
void annotate(Value l, char* annotation); 
void print(Value v);

// builtins; probably should move to different header
Value symbol(char *buf);
Value number(int n);
Value nil();
Value truth();
bool empty(Value v);
bool is_true(Value v);
Value cons(Value head, Value tail);
Value head(Value v);
Value tail(Value v);
Value plus(Value a, Value b);
Value minus(Value a, Value b);
Value atom(Value arg);
Value eq(Value a, Value b);
Value let(Value symbol, Value binding, Value table);
Value lambda(Value symbol, Value body, Value env);
Value cond(Value condition, Value consequent, Value alternate, Value env);
