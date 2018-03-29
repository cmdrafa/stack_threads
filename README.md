# stack_threads
Stack implementation using  the POSIX threads lib.

non_cond is the implementation using only locks and no condition variables. 

Compile it using adequate flags (including the -pthread). To run you must pass the stack size as the first argument and the number of threads as the second argument
