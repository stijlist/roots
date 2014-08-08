// this file implements a dictionary

/* dictionaries have a simple API:
 * assoc 
 * dissoc 
 * lookup 
 * each of these returns a Value struct */

/* what is the internal representation of a dictionary? 
 * we could do the simplest possible thing and have a 
 * lisp-style alist (list of pairs, where the first element
 * is a key and the second element is a value. o(n) lookup, 
 * o(1) insert)
 * 
 * we could build a hashtable where the internal storage type
 * is an array of Value structs, and we index into it using
 * some hash function like "xor all the bits together, and 
 * modulo the result by the size of the array." Imagine an 
 * 256 element array: we'd xor the bytes of each value we 
 * wanted to assoc or dissoc and use that as the index to 
 * store the element in. This might work. Is this a better 
 * approach than the above? 
 *
 * What are the design goals for this hash table? 
 * well... it would be really flipping sweet if we didn't have
 * to malloc or free to use it... bro. can we just copy values
 * around? 
 *
 * what are the semantics of value copying? 
 *
 * we use this hashtable for the environment passed to lambda.
 * what is the flow of state for the environment? 
 * what if it was stack-allocated?
 *
 * run => (eval body (make-env)) -> make-env allocates on the 
 * stack, and we have access to it for the entirety of eval
 * which by definition is our entire program
 *
 * what about statically scoping things? am I getting scope 
 * wrong with this conception of binding? 
 * */

