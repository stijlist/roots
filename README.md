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
- 4 space -> 2 space formatting

TODO (in rough dependency order):
- macro
- fn (more than 1 arg)
- real arithmetic
- cond
- proper error handling / program termination
- garbage collection
