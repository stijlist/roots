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
- arithmetic
  - plus
  - minus

TODO (in rough dependency order):
- better printing of dotted pairs
- arithmetic
  - multiply
  - divide
- macro
- fn (more than 1 arg)
- cond
- proper error handling / program termination
- garbage collection
