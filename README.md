This is a toy lisp implementation that will be used to build towards a logic
programming and functional programming system. I want to test how well I under-
stand the mechanical implications of my chosen programming paradigms. I will 
attempt to test the correctness of my programs using a quickcheck-style genera-
tive tester. A simple roadmap:        

- Implement cons, car, cdr, quote, cond, eq?, atom? in C
    - alternate branch: implement in Rust
- Fuzz test my implementations of each of these functions using a Haskell
  driver
- Implement a metacircular evaluator in the resulting Lisp  
    - alternate branch: implement Kernel lisp
- Implement miniKanren in the resulting Lisp
    - alternate branch: implement OMeta in the resulting Lisp
