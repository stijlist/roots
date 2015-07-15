#ifndef ROOTS_H
#define ROOTS_H
#include "roots.h"
#define MAX_SYMBOL_SIZE 32

Value symbol(char *buf) {
    return (Value) { (Data) buf, Symbol };
}

Value number(int n) {
    return (Value) { (Data) n, Number };
}

Value nil() {
    return (Value) { (Data) 0, Nil };
}

// design flaw? should I need to tag truthy values? 
// NOTE: reexamine this after you've implemented define
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

Value head(Value v) {
    if (v.tag != ConsCell && v.tag != Lambda) {
        printf("Error, calling head on a non-list value.\n");
        return nil();
    } else {
        return v.data.list->head;
    }
}
    
Value tail(Value v) {
    if (v.tag != ConsCell && v.tag != Lambda)
        printf("Error, calling tail on a non-list value.\n");
    else
        return v.data.list->tail;

    return nil();
}

Table make_env() {
    Table env;
    for (int i=0; i < 256; i++) {
        env.key[i].head = nil();
        env.key[i].tail = nil();
    }
    return env;
}


// not certain of the correctness of this function
bool is_empty(Value v) {
    return v.tag == ConsCell && head(v).tag == Nil && tail(v).tag == Nil;
}

// potential optimization: store symbols in a symbol table
bool streq(char *str1, char *str2) {
    for (int i=0; (str1[i] != '\0' && str2[i] != '\0'); i++)
        if (str1[i] != str2[i]) return false;

    return true;
}

bool symeq(Value sym, char *str) {
    return sym.tag == Symbol && streq(sym.data.symbol, str);
}

Value lambda(Value symbol, Value body) {
    Value l = cons(symbol, body); // representing lambdas as simply as possible
    l.tag = Lambda;
    return l;
}

// this is the wrong implementation of lambda; we should take an env
// as an argument, and create a new env with the old env + the new 
// bindings, and eval all the things with that new env
Value apply(Value lambda_pair, Value arg) {
    // swap the arg into all occurrences of symbol in the lambda body
    // representing lambdas as a simple (symbol, body) pair for now
    Value symbol = head(lambda_pair);
    Value body = tail(lambda_pair);
    if (body.tag == ConsCell) {
        return cons(apply(lambda(symbol, head(body)), arg),
                    apply(lambda(symbol, tail(body)), arg));
    } else if (body.tag == Symbol && symeq(body, symbol.data.symbol)) {
        return arg;
    } else {
        return body;
    }
}

Value quote(Value arg) {
    return arg;
}

Value atom(Value arg) {
    if (is_empty(arg) || arg.tag == Nil || arg.tag == Symbol)
        return truth();
    else
        return nil();
}

Value eq(Value arg1, Value arg2) {
    if (arg1.tag == arg2.tag) {
        if (arg1.tag == Symbol && 
            streq(arg1.data.symbol, arg2.data.symbol)) {
            return truth();
        } else if (arg1.tag == ConsCell && is_empty(arg1) && is_empty(arg2)) {
            return truth();
        } else if (arg1.tag == Number) {
            return (arg1.data.number == arg2.data.number) ? truth() : nil();
        }
    }
    return nil();
}

Value car(Value arg) {
    return head(arg);
}

Value cdr(Value arg) {
    return tail(arg);
}

// why is cond the only builtin that needs to take `env`? revisit this
Value cond(Value condition, Value consequent, Value alternate, Table env) {
    return eval(condition, env).tag == Truth ? eval(consequent, env) : eval(alternate, env);
}

Value lookup(Value symbol, Table table) {
    for(int i=0; i < 256; i++) {
        if (table.key[i].head.tag == Nil) {
            break;
        }
        if (symeq(symbol, table.key[i].head.data.symbol)) { // is it just me or is this gross 
            return table.key[i].tail;
        }
    }

    return nil();
}

Table let(Value symbol, Value binding, Table table) {
    Table new_env;
    for(int i=0; i < 256; i++) {
        Cons current_pair = table.key[i];
        if (current_pair.head.tag == Nil) {
            new_env.key[i].head = symbol;
            new_env.key[i].tail = binding;
            break;
        } else if (current_pair.head.tag == Symbol && symeq(symbol, current_pair.head.data.symbol)) {
            new_env.key[i].tail = binding;
            break;
        } else { // copy over values that aren't changing
            new_env.key[i].head = table.key[i].head;
            new_env.key[i].tail = table.key[i].tail;
        }
    }

    // printEnv(new_env);
    return new_env;
}

// i have a hunch that pass-by-value here simplifies the implementation of closures
// all builtins are fixed-arity; we can implement variable arity as macros
Value eval(Value arg, Table env) {
    if (arg.tag == ConsCell) {
        Value operator = car(arg);
        Value operands = cdr(arg);
        // should probably be checking
        // if(atom(operator))
        if (operator.tag == ConsCell) {
          operator = eval(operator, env);
        }

        if (symeq(operator, "quote")) {
            // only takes one argument; ignores the rest
            // doesn't evaluate its arguments
            // (quote (1 2 3)) => (1 2 3)
            // (quote (1 2 3) 4) => (1 2 3)
            return quote(car(operands));
        } else if (symeq(operator, "atom")) {
            // only takes one argument
            return atom(eval(car(operands), env));
        } else if (symeq(operator, "eq")) {
            // (eq 1 1) => t
            return eq(eval(car(operands), env), eval(car(cdr(operands)), env));
        } else if (symeq(operator, "car")) {
            // (car (1 2 3)) => 1
            Value arg = car(operands);
            return car(eval(arg, env));
        } else if (symeq(operator, "cdr")) {
            // (cdr (1 2 3)) => (2 3)
            Value arg = car(operands);
            return cdr(eval(arg, env));
        } else if (symeq(operator, "cons")) {
            // (cons 1 (2 3)) => (1 2 3)
            return cons(eval(car(operands), env), eval(car(cdr(operands)), env));
        } else if (symeq(operator, "if")) {
            // (if t 1 0) => 1
            Value test = car(operands);
            Value consequent = car(cdr(operands));
            Value alternate = car(cdr(cdr(operands)));
            return cond(test, consequent, alternate, env);
        } else if (symeq(operator, "lambda")) {
            // ((lambda x (cons 1 x)) 2) => (1 2)
            // (((lambda x (lambda y (cons x y))) 1) 2) => (1 2)
            Value symbol = car(operands);
            Value body = car(cdr(operands));
            return lambda(symbol, body);
        } else if (operator.tag == Lambda) {
            return eval(apply(operator, car(operands)), env);
        } else if (symeq(operator, "let")) {
            // (let (x 1) x) => 1
            Value bindings = car(operands);
            Value symbol = car(bindings);
            Value value = eval(car(cdr(bindings)), env);
            Value body = car(cdr(operands));
            Table new_env = let(symbol, value, env);
            return eval(body, new_env);
        } else {
          printValue(operator);
          printf(" is not a function.\n");
        }
    } else {
        if (arg.tag == Symbol) {
            return lookup(arg, env);
        } else { // numbers and nils evaluate to themselves
            return arg;
        }
    }
    // returning nil in failure cases until I figure out exception handling
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

ParseResult parsesym(char *cursor) {
    char *symbuf = malloc(MAX_SYMBOL_SIZE * sizeof(char));
    for (int i = 0; is_alpha(*cursor); cursor++, i++) 
        symbuf[i] = *cursor;

    Value value = streq(symbuf, "t") ? truth() : symbol(symbuf);
    return (ParseResult) { cursor, value };
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

ParseResult parselist(char *cursor) {
    if (is_close_paren(*cursor)) {
        return (ParseResult) { ++cursor, nil() };
    } else { 
        ParseResult result = parse(cursor);
        ParseResult remaining = parselist(result.newcursor);
        Value rest = cons(result.value, remaining.value);
        return (ParseResult) { remaining.newcursor, rest };
    }
}

ParseResult parse(char *cursor) {
    cursor = next_value_at(cursor);
    if (is_open_paren(*cursor)) {
        return parselist(++cursor); // consume a paren
    } else if (is_alpha(*cursor)) {
        return parsesym(cursor);
    } else if (is_num(*cursor)) {
        return parsenum(cursor);
    } else if (*cursor == 0) {
        return (ParseResult) { cursor, nil() };
    } else {
        printf("Problem reading buffer at:\n %s\n", cursor);
        return (ParseResult) { cursor, nil() };
    }
}

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
            printf("()");
            break;
        case Lambda:
            printf("(lambda ");
            printValue(head(v));
            printf(" ");
            printValue(tail(v));
        case Truth:
            printf("t");
    }
}

void printList(Value l) {
    printf("(");
    while(l.tag == ConsCell) {
      printValue(head(l));
      l = tail(l);
      if (l.tag != Nil) printf(" ");
    }
    if (l.tag != Nil) printValue(l);
    printf(")");
}

void printEnv(Table t) {
    printf("Env: ");
    for (int i=0; i < 256; i++) {
      printf("\n");
      printf("Symbol #%d: ", i);
      printValue(t.key[i].head);
      printf("\n");
      printf("Binding: ");
      printValue(t.key[i].tail);
    }
    printf("End env.");
    printf("\n");
}
#endif
