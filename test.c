#include "roots.h"

void read_eval_print(char *list) {
    printf("input: %s\n", list); 
    Value v = eval_empty(read(list));
    annotate(v, "=> ");
}

void assert_equal(char *actual_str, char *expected_str) {
      Value actual = eval_empty(read(actual_str));
      Value expected = read(expected_str);
      if (is_true(eq(actual, expected))) {
          printf("ok: %s\n", actual_str);
      } else {
          annotate(expected, "not ok: expected: ");
          annotate(actual, "actual: ");
      }
}

int main() {
    printf("Hello world.\n\n");

    assert_equal("(cons 1 2)", "(1 2)");
    read_eval_print("(cons 1 (quote (2 3)))");
    read_eval_print("(cons (quote (1 2)) 3)");
    read_eval_print("(head (quote (1 2)))");
    read_eval_print("(tail (quote (1 2)))");
    read_eval_print("(quote (1 2))");
    read_eval_print("(eq (quote foo) (quote foo))");
    read_eval_print("(eq (quote foo) (quote bar))");
    read_eval_print("(eq 1 1)");
    read_eval_print("(eq (head (quote (1))) 1)");

    read_eval_print("(head (quote (foo bar)))");

    read_eval_print("(eq 1 (head (quote (1 2))))");
    read_eval_print("(eq (quote foo) (head (cons (quote foo) (quote bar))))");

    read_eval_print("(atom (quote foo))");
    read_eval_print("(atom (quote (1 2)))");

    read_eval_print("(if t 1 0)");
    read_eval_print("(if (atom ()) (cons 1 2) 0)");
    read_eval_print("(if (atom (quote (1 2))) (cons 1 2) 0)");

    read_eval_print("(let (n 1) n)");
    read_eval_print("(let (n 1) (cons n 1))");
    read_eval_print("(let (n 1) (cons n n))");
    read_eval_print("(let (x (cons 1 2)) x)");
    read_eval_print("(let (x (cons 1 2)) (cons x 3))");
    read_eval_print("(let (x 1) (let (y 2) (cons x y)))");

    read_eval_print("((lambda x x) 2)");
    read_eval_print("((lambda x (cons 1 x)) 2)");

    read_eval_print("((lambda x ((lambda y (cons x y)) 2)) 1)");
    read_eval_print("((lambda x ((lambda x (cons x x)) 2)) 1)");
    
    read_eval_print("(let (f (lambda x x)) (f 1))");
    read_eval_print("(lambda x x)");
    read_eval_print("((lambda x (lambda y x)) 1)");
    read_eval_print("(((lambda x (lambda y x)) 1) 2)");
    read_eval_print("(let (f (lambda x (lambda y (cons x y)))) ((f 1) 2))");

    read_eval_print("(let (f (lambda x (if (eq x 0) (f 1) x))) (f 0))");
    // (let (nth (lambda list (lambda n (if (eq n 0) (head list) ((nth (tail list)) (minus n 1)))))))
    read_eval_print("(eq (quote (t)) (quote (t)))");
    return 0;
}
