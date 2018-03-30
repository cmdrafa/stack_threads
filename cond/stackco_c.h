#include <pthread.h>
#ifndef _stackco_c_h
#define _stackco_c_h

struct stack_el
{ // element of a stack
	char *str;
	int n;
};

struct stack
{						  // a stack
	struct stack_el **sp; // pointer to array to store stack elements
	int size;			  // size of the array
	int top;			  /** array index of the element at the top of the stack, i.e. the first free position on the stack -- initially 0 **/
	pthread_mutex_t lock; // Mutex for locking
	pthread_cond_t empty;
	pthread_cond_t full; // two differente conditions for signaling
};

struct stack *Init_Stack(int size);					   // initialize stack. Returns pointer to initialized stack of desired size
int Push_Stack(struct stack *sp, struct stack_el *el); // returns 0 if the stack is full, and 1 otherwise
struct stack_el *Pop_Stack(struct stack *sp);		   // returns NULL if the stack is empty otherwise the pointer to the element that was on top of the stack
void Show(struct stack *sp);						   // print all elements of the stack
void Free_Stack(struct stack *sp);					   // frees all data structures allocated

#endif