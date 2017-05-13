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
- inspect
- variadic functions
- repl works on multiline functions
- cond
- program termination with nonzero return in error cases

TODO:
- bootstrap
- strings
- garbage collection


known bugs:

Side-effects in "if" or "cond" forms don't work:

```
  (define l (quote (1 2)))
  (if (inspect (cons 3 l)) 1 0)
  (inspect l)
```

Updated testing strategy:

- input and expected output files
- shell script that executes the input files with the current version of roots and runs diff on the input and output, printing the filenames of the diffs
- git precommit hook that updates the diffs
