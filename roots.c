#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define INPUT_BUFFER_SIZE 1024

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
  if (v.tag == ConsCell) {
    return v.data.list->head;
  } else {
    annotate(v, "Error, calling head on a non-list value: ");
    return nil();
  }
}
    
Value tail(Value v) {
  if (v.tag == ConsCell) {
    return v.data.list->tail;
  } else {
    annotate(v, "Error, calling tail on a non-list value: ");
    return nil();
  }
}

bool empty(Value v) {
  return v.tag == Nil || (v.tag == ConsCell && head(v).tag == Nil && tail(v).tag == Nil);
}

bool is_true(Value v) {
  return !empty(v);
}

bool streq(char *str1, char *str2) {
  int i = 0;
  for (; (str1[i] != '\0' && str2[i] != '\0'); i++)
    if (str1[i] != str2[i]) return false;

  return str1[i] == str2[i]; // check that the last char is equal in case one terminated early
}

bool symeq(Value sym, char *str) {
  return sym.tag == Symbol && streq(sym.data.symbol, str);
}

bool ensure_numeric_op(Value a, Value b, char *op) {
  if (a.tag != Number || b.tag != Number) {
    printf("Tried to perform %s on non-numeric pair: ", op);
    print(cons(a, b));
    printf("\n");
    return false;
  } else {
    return true;
  }
}

Value add(Value a, Value b) {
  return ensure_numeric_op(a, b, "addition") ? 
    number(a.data.number + b.data.number) : nil();
}

Value subtract(Value a, Value b) {
  return ensure_numeric_op(a, b, "subtraction") ? 
    number(a.data.number - b.data.number) : nil();
}

Value multiply(Value a, Value b) {
  return ensure_numeric_op(a, b, "multiplication") ? 
    number(a.data.number * b.data.number) : nil();
}

Value divide(Value a, Value b) {
  return ensure_numeric_op(a, b, "division") ? 
    number(a.data.number / b.data.number) : nil();
}

Value atom(Value arg) {
  return (empty(arg) || arg.tag == Symbol) ? truth() : nil();
}

Value eq(Value a, Value b) {
  if (a.tag == b.tag) {
    if (a.tag == Truth) {
      return truth();
    } else if (empty(a) && empty(b)) {
      return truth();
    } else if (a.tag == Number) {
      return (a.data.number == b.data.number) ? truth() : nil();
    } else if (a.tag == Symbol && streq(a.data.symbol, b.data.symbol)) {
      return truth();
    } else if (a.tag == ConsCell && !empty(a) && !empty(b)) {
      bool heads_eq = is_true(eq(head(a), head(b)));
      bool tails_eq = is_true(eq(tail(a), tail(b)));
      return (heads_eq && tails_eq) ? truth() : nil();
    }
  }
  return nil();
}

Value lookup(Value symbol, Value table) {
  if(!empty(table)) {
    if (is_true(eq(head(head(table)), symbol))) {
      return tail(head(table));
    } else {
      return lookup(symbol, tail(table));
    }
  }
          
  annotate(symbol, "Failed to find symbol ");
  return nil();
}

Value let(Value symbol, Value binding, Value table) {
  if (!empty(table)) {
    if (is_true(eq(head(head(table)), symbol))) {
      return cons(cons(symbol, binding), tail(table));
    } else {
      return cons(head(table), let(symbol, binding, tail(table)));
    }
  } else {
    return cons(cons(symbol, binding), nil());
  }
}

Value merge(Value base, Value overlay) {
  if (empty(overlay)) {
    return base;
  } else {
    Value overlay_pair = head(overlay);
    Value new_base = let(head(overlay_pair), tail(overlay_pair), base);
    Value overlay_tail = tail(overlay);
    return merge(new_base, overlay_tail);
  }
}

Value lambda(Value bindings, Value body, Value env) {
  Closure* closure = malloc(sizeof(Closure));
  closure->bindings = bindings;
  closure->body = body;
  closure->env = env;
  return (Value) { (Data) closure, Lambda };
}

Value apply(Value lambda, Value args, Value env) {
  Value bindings = lambda.data.closure->bindings, arg = args;
  Value merged_env = merge(env, lambda.data.closure->env);
  while(!empty(bindings) && !empty(arg)) {
    merged_env = let(head(bindings), head(arg), merged_env);
    bindings = tail(bindings), arg = tail(arg);
  }
  return eval(lambda.data.closure->body, merged_env);
}

Value ternary(Value condition, Value consequent, Value alternate, Value env) {
  // TODO: eval_mutating_env in the condition?
  return is_true(eval(condition, env)) ? eval(consequent, env) : eval(alternate, env);
}

Value first(Value list) { return head(list); }
Value second(Value list) { return head(tail(list)); }
Value third(Value list) { return head(tail(tail(list))); }

Value inspect(Value v) {
  print(v);
  printf("\n");
  return v;
}

Value eval_all(Value args, Value env) {
  return empty(args) ? nil() : cons(eval(head(args), env), eval_all(tail(args), env));
}

Value cond(Value args, Value env) {
  Value condition = first(head(args)), result = second(head(args));
  return is_true(eval(condition, env)) ? eval(result, env) : cond(tail(args), env);
}

Value eval(Value arg, Value env) {
  if (arg.tag == ConsCell) {
    Value operator = head(arg), operands = tail(arg), fn;

    if (operator.tag == ConsCell) {
      return eval(cons(eval(operator, env), operands), env);
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
      return ternary(first(operands), second(operands), third(operands), env);
    } else if (symeq(operator, "cond")) {
      return cond(operands, env);
    } else if (symeq(operator, "lambda")) {
      return lambda(first(operands), second(operands), env);
    } else if (symeq(operator, "inspect")) {
      return inspect(eval(first(operands), env));
    } else if (symeq(operator, "+")) {
      return add(eval(first(operands), env), eval(second(operands), env));
    } else if (symeq(operator, "-")) {
      return subtract(eval(first(operands), env), eval(second(operands), env));
    } else if (symeq(operator, "*")) {
      return multiply(eval(first(operands), env), eval(second(operands), env));
    } else if (symeq(operator, "/")) {
      return divide(eval(first(operands), env), eval(second(operands), env));
    } else if (symeq(operator, "let")) {
      Value bindings = first(operands);
      Value new_env = let(first(bindings), eval(second(bindings), env), env);
      return eval(second(operands), new_env);
    } else if (operator.tag == Lambda) {
      return apply(operator, operands, env);
    } else if ((fn = lookup(operator, env)).tag == Lambda){
      return apply(fn, eval_all(operands, env), env);
    } else {
      annotate(operator, "Not a function: ");
    }
  } else {
    if (arg.tag == Symbol) {
      return lookup(arg, env);
    } else { // numbers and nils evaluate to themselves
      return arg;
    }
  }
  printf("Error, exiting.");
  exit(1);
}

Value eval_mutating_env(Value arg, Value* env) {
  if (arg.tag == ConsCell && symeq(head(arg), "define")) {
    Value operands = tail(arg);
    Value binding = eval(second(operands), *env);
    *env = let(first(operands), binding, *env);
    return binding;
  } else {
    return eval(arg, *env);
  }
}

typedef struct _parseresult {
  char *newcursor;
  Value value;
} ParseResult;

ParseResult parse(char *cursor);

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

bool is_sym(char c) {
  return (c >= 33 && c <= 39) // !"#$%&'
      || (c >= 42 && c <= 47) // *+,-./
      || (c >= 58 && c <= 64) // :;0<=>?@
      || (c >= 91 && c <= 96) // [\]^_`
      || (c >= 123 && c <= 126); // {|}~
}

bool valid_symbol_char(char c) {
  return is_alpha(c) || is_sym(c);
}

ParseResult parsesym(char *cursor) {
  char *symbuf = malloc(MAX_SYMBOL_SIZE * sizeof(char));
  for (int i = 0; valid_symbol_char(*cursor); cursor++, i++) 
    symbuf[i] = *cursor;

  Value value = streq(symbuf, "t") ? truth() : symbol(symbuf);
  return (ParseResult) { cursor, value };
}

char* next_value_at(char *cursor) {
  while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n') 
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
  } else if (*cursor == '\0') {
    printf("Unterminated '(', stopping parse.");
    exit(1);
    return (ParseResult) { cursor, nil() };
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
  } else if (valid_symbol_char(*cursor)) {
    return parsesym(cursor);
  } else if (is_num(*cursor)) {
    return parsenum(cursor);
  } else {
    printf("Problem reading buffer at:\n %s\n", cursor);
    exit(1);
  }
}

Value read(char* string) {
  return parse(string).value;
}

void print_list_contents(Value list) {
  if (tail(list).tag == Nil) {
    print(head(list));
  } else if (tail(list).tag == ConsCell) {
    print(head(list));
    printf(" ");
    print_list_contents(tail(list));
  } else { // dotted
    printf("(");
    print(head(list));
    printf(" . ");
    print(tail(list));
    printf(")");
  }
}

void print(Value current) {
  switch (current.tag) {
    case Number:
      printf("%d", current.data.number);
      break;
    case Symbol:
      printf("%s", current.data.symbol);
      break;
    case ConsCell:
      printf("(");
      print(head(current));
      if (tail(current).tag == Nil) {
        printf(")");
      } else if (tail(current).tag == ConsCell) {
        printf(" ");
        print_list_contents(tail(current)); // no parens
        printf(")");
      } else { // dotted
        printf(" . ");
        print(tail(current));
        printf(")");
      }
      break;
    case Nil:
      printf("()");
      break;
    case Lambda:
      printf("(");
      print(current.data.closure->bindings);
      printf(" => ");
      print(current.data.closure->body);
      printf(" in ");
      print(current.data.closure->env);
      printf(")");
      break;
    case Truth:
      printf("t");
      break;
  }
}

void annotate(Value v, char* annotation) { 
  printf("%s", annotation);
  print(v);
  printf("\n");
}

char* get_line() {
  char* s = malloc(INPUT_BUFFER_SIZE * sizeof(char));
  return fgets(s, INPUT_BUFFER_SIZE, stdin);
}

bool balanced(char* expr) {
  char open_paren = 40, close_paren = 41;

  int parenlevel = 0;
  for (char *c = expr; *c != '\0' && *c != EOF; c++) {
    if (*c == open_paren) parenlevel++;
    if (*c == close_paren) parenlevel--;
    if (parenlevel < 0) break;
  }

  return parenlevel == 0;
}

void repl() {
  char *input, *more;
  Value env = nil();
  while ((input = get_line()) != NULL) {

    while (!balanced(input) && (more = get_line()) != NULL) {
      strlcat(input, more, INPUT_BUFFER_SIZE);
    }

    print(eval_mutating_env(read(input), &env));
    printf("\n");
  } 
}

#endif
