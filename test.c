#include "roots.h"

void test_unbalanced() {
  printf("testing 'unbalanced' utility function:\n");
  printf("\tshould be true: %s\n", unbalanced("()") ? "true" : "false");
  printf("\tshould be false: %s\n", unbalanced("(") ? "true" : "false");
}

void test(char *input, char *expected_str) {
  Value actual = eval(read(input), nil());
  Value expected = read(expected_str);
  if (!empty(eq(actual, expected))) {
    printf("ok: %s\n", input);
  } else {
    annotate(expected, "not ok, expected: ");
    printf("input: %s\n", input);
    annotate(actual, "actual: ");
  }
}

int main() {
  printf("Hello world.\n\n");

  test_unbalanced();

  test("()", "()");
  // don't have a way to read dotted pair literals yet
  test("(cons 1 ())", "(1)");
  test("(quote (1 2))", "(1 2)");
  test("(cons 1 (quote (2 3)))", "(1 2 3)");
  // don't have a way to read dotted pair literals yet
  test("(cons (quote (1 2)) (cons 3 ()))", "((1 2) 3)");
  test("(head (quote (1 2)))", "1");
  test("(tail (quote (1 2)))", "(2)");
  test("(quote (1 2))", "(1 2)");
  test("(eq (quote foo) (quote foo))", "t");
  test("(eq (quote foo) (quote bar))", "()");
  test("(eq 1 1)", "t");
  test("(eq (head (quote (1))) 1)", "t");
  test("(eq (quote (1 2)) (quote (1 2)))", "t");
  test("(eq (cons 1 2) (cons 1 2))", "t");

  test("(head (quote (foo bar)))", "foo");

  test("(eq 1 (head (quote (1 2))))", "t");
  test("(eq (quote foo) (head (cons (quote foo) (quote bar))))", "t");

  test("(atom (quote foo))", "t");
  test("(atom (quote (1 2)))", "()");

  test("(if t 1 0)", "1");
  test("(if (atom ()) (quote (1 2)) 0)", "(1 2)");
  test("(if (atom (quote (1 2))) (cons 1 2) 0)", "0");

  test("(let (n 1) n)", "1");
  test("(let (n 1) (cons n ()))", "(1)");
  test("(let (x (quote (1 2))) x)", "(1 2)");
  test("(let (x 1) (let (y 2) (cons x (cons y ()))))", "(1 2)");

  test("((lambda (x) x) 2)", "2");
  test("((lambda (x) (cons x ())) 1)", "(1)");

  test("((lambda (x y) (cons x (cons y ()))) 1 2)", "(1 2)");
  test("(let (f (lambda (x y) (cons x (cons y ())))) (f 1 2))", "(1 2)");
  test("(let (f (lambda (x y z) (cons x (cons y (cons z ()))))) (f 1 2 3))", "(1 2 3)");

  test("((lambda (x) ((lambda (y) (cons x (cons y ()))) 2)) 1)", "(1 2)");
  test("((lambda (x) ((lambda (x) x) 2)) 1)", "2");
  test("(let (f (lambda (x) x)) (f 1))", "1");
  test("(((lambda (x) (lambda (y) x)) 1) 2)", "1");
  test("(let (f (lambda (x) (lambda (y) (cons x (cons y ()))))) ((f 1) 2))", "(1 2)");

  test("(let (f (lambda (x) x)) (f (cons 1 ())))", "(1)");
  test("(let (f (lambda (x) (if (eq x 0) (f 1) x))) (f 0))", "1");
  test("(let (fib (lambda (n) (if (eq n 1) 1 (if (eq n 2) 1 (+ (fib (- n 1)) (fib (- n 2))))))) (fib 5))", "5");
  return 0;
}
