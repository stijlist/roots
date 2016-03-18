This is a toy lisp implementation loosely based on Paul Graham's [The Roots of Lisp](http://ep.yimg.com/ty/cdn/paulgraham/jmc.ps) (postscript file).

Working:

- car (head)
- cdr (tail)
- cons
- if
- quote
- atom
- eq
- let
- lambda
  - recursion
  - lexical scope
  - closures
- define (for repl only)
- integer arithmetic
  - add
  - subtract
  - multiply
  - divide
- non-alphabetical symbols
- variadic functions
- repl works on multiline functions
- cond
- program termination with nonzero return in error cases

TODO:
- strings
- garbage collection
- bootstrap


known bugs:

Side-effects in "if" or "cond" forms don't work.
