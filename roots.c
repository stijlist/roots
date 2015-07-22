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

// could use an arbitrary symbol to indicate truth and return true for all 
// non-nil / empty values but then it would be possible to redefine that 
// particular symbol
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
    // lambdas are implemented using the cons cell structure but a different tag
    if (v.tag == ConsCell || v.tag == Lambda) {
        return v.data.list->head;
    } else {
        printf("Error, calling head on a non-list value.\n");
        return nil();
    }
}
    
Value tail(Value v) {
    // lambdas are implemented using the cons cell structure but a different tag
    if (v.tag == ConsCell || v.tag == Lambda)
        return v.data.list->tail;
    else
        printf("Error, calling tail on a non-list value.\n");

    return nil();
}

// nils and dotted pairs of nils are interchangeable for nil-punning
bool is_empty(Value v) {
    return v.tag == Nil || (v.tag == ConsCell && head(v).tag == Nil && tail(v).tag == Nil);
}

bool is_true(Value v) {
    return v.tag == Truth;
}

bool streq(char *str1, char *str2) {
    for (int i=0; (str1[i] != '\0' && str2[i] != '\0'); i++)
        if (str1[i] != str2[i]) return false;

    return true;
}

bool symeq(Value sym, char *str) {
    return sym.tag == Symbol && streq(sym.data.symbol, str);
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

Value lookup(Value symbol, Value table) {
    if(!is_empty(table)) {
        if (is_true(eq(head(head(table)), symbol))) {
            return tail(head(table));
        } else {
            return lookup(symbol, tail(table));
        }
    }
          
    printf("Failed to find symbol ");
    printValue(symbol);
    printf("\n");
    return nil();
}

Value let(Value symbol, Value binding, Value table) {
    if (!is_empty(table)) {
        if (is_true(eq(head(head(table)), symbol))) {
            return cons(cons(symbol, binding), tail(table));
        } else {
            return cons(head(table), let(symbol, binding, tail(table)));
        }
    } else {
        return cons(cons(symbol, binding), nil());
    }
}

Value lambda(Value symbol, Value body) {
    Value l = cons(symbol, body);
    l.tag = Lambda;
    return l;
}

Value apply(Value lambda_pair, Value arg, Value env) {
    Value new_env = let(head(lambda_pair), arg, env);
    return eval(tail(lambda_pair), new_env);
}

Value cond(Value condition, Value consequent, Value alternate, Value env) {
    return is_true(eval(condition, env)) ? eval(consequent, env) : eval(alternate, env);
}

Value eval_empty(Value arg) {
  return eval(arg, cons(nil(), nil()));
}

Value first(Value list) { return head(list); }
Value second(Value list) { return head(tail(list)); }
Value third(Value list) { return head(tail(tail(list))); }

// all builtins are fixed-arity; we can implement variable arity as macros
Value eval(Value arg, Value env) {
    if (arg.tag == ConsCell) {
        Value operator = head(arg);
        Value operands = tail(arg);

        // if the operator is a list, try and reduce it to an atom
        if (operator.tag == ConsCell) {
          operator = eval(operator, env);
        }

        if (operator.tag == Lambda) {
            return apply(operator, first(operands), env);
        } else if (symeq(operator, "quote")) {
            return first(operands);
        } else if (symeq(operator, "atom")) {
            return atom(eval(first(operands), env));
        } else if (symeq(operator, "eq")) {
            return eq(eval(first(operands), env), eval(second(operands), env));
        } else if (symeq(operator, "head")) {
            return head(eval(first(operands), env));
        } else if (symeq(operator, "tail")) {
            return tail(eval(first(operands), env));
        } else if (symeq(operator, "cons")) {
            return cons(eval(first(operands), env), eval(second(operands), env));
        } else if (symeq(operator, "if")) {
            return cond(first(operands), second(operands), third(operands), env);
        } else if (symeq(operator, "lambda")) {
            return lambda(first(operands), second(operands));
        } else if (symeq(operator, "let")) {
            Value bindings = first(operands);
            Value new_env = let(first(bindings), eval(second(bindings), env), env);
            return eval(second(operands), new_env);
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
        ParseResult result = read(cursor);
        ParseResult remaining = parselist(result.newcursor);
        Value rest = cons(result.value, remaining.value);
        return (ParseResult) { remaining.newcursor, rest };
    }
}

ParseResult read(char *cursor) {
    cursor = next_value_at(cursor);
    if (is_open_paren(*cursor)) {
        return parselist(++cursor); // consume a paren
    } else if (is_alpha(*cursor)) {
        return parsesym(cursor);
    } else if (is_num(*cursor)) {
        return parsenum(cursor);
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
            break;
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
#endif
