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
- define (only works in repl)

TODO (in rough dependency order):

- arithmetic
- define
- macro
- fn (more than 1 arg)
- cond
- proper error handling / program termination
- garbage collection
