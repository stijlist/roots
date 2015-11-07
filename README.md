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

TODO (in rough dependency order):
- variadic functions
- cond
- bootstrap
- proper error handling / program termination
- garbage collection
