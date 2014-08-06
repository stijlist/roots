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
// TODO: reexamine this after you've implemented define
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
    if (v.tag != ConsCell) {
        printf("Error, calling head on a non-list value.\n");
        return nil();
    } else {
        return v.data.list->head;
    }
}
    
Value tail(Value v) {
    if (v.tag != ConsCell)
        printf("Error, calling tail on a non-list value.\n");
    else
        return v.data.list->tail;

    return nil();
}

// not certain of the correctness of this function
bool is_empty(Value v) {
    return v.tag == ConsCell && head(v).tag == Nil && tail(v).tag == Nil;
}

// potential optimization: store symbols in a symbol table
bool streq(char *str1, char *str2) {
    for (int i=0; (str1[i] != 0 && str2[i] != 0); i++)
        if (str1[i] != str2[i]) return false;

    return true;
}

bool symeq(Value sym, char *str) {
    return streq(sym.data.symbol, str);
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

Value cond(Value condition, Value consequent, Value alternate) {
    return eval(condition).tag == Truth ? eval(consequent) : eval(alternate);
}

Value eval(Value arg) {
    if (arg.tag == ConsCell) {
        Value operator = car(arg);
        Value operands = cdr(arg);

        if (symeq(operator, "quote")) {
            // only takes one argument; ignores the rest
            // (quote (1 2 3)) => (1 2 3)
            // (quote (1 2 3) 4) => (1 2 3)
            return quote(car(operands));
        } else if (symeq(operator, "atom")) {
            // only takes one argument
            return atom(car(operands));
        } else if (symeq(operator, "eq")) {
            // only takes two arguments. should I make this variadic?
            // (eq 1 1) => t
            return eq(car(operands), car(cdr(operands)));
        } else if (symeq(operator, "car")) {
            // only takes one argument, a list
            // (car (1 2 3)) => 1
            Value list = car(operands);
            return car(list);
        } else if (symeq(operator, "cdr")) {
            // only takes one argument
            // (cdr (1 2 3)) => (2 3)
            Value list = car(operands);
            return cdr(list);
        } else if (symeq(operator, "cons")) {
            // only takes two arguments
            // (cons 1 (2 3)) => (1 2 3)
            return cons(car(operands), cdr(operands));
        } else if (symeq(operator, "if")) {
            // only takes three arguments
            // mccarthy's original lisp implemented cond (arbitrary # arguments)
            // (if t 1 0) => 1
            Value test = car(operands);
            Value options = cdr(operands);
            return cond(test, car(options), cdr(options));
        } else if (symeq(operator, "lambda")) {
            // ???
            printf("Lambda not implemented. \n");
        }
    } else {
        // numbers and nils evaluate to themselves
        return arg;
    }
    // returning nil in failure cases until I figure out exception handling
    return nil();
}

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
        case Truth:
            printf("t");
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
#endif
