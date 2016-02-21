#include "utils.h"

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
  Value bindings;
  Value body;
  Value env;
};

Value read(char *string);
Value eval(Value arg, Value env);
Value eval_mutating_env(Value arg, Value* env);
void print(Value v);
void annotate(Value l, char* annotation); 

Value nil();
bool empty(Value v);
Value eq(Value a, Value b);
