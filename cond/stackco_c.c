#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stackco_c.h"

struct stack *Init_Stack(int size)
{
    printf("Initialzing stack\n");
    struct stack *sp = malloc(sizeof(*sp));
    pthread_mutex_init(&sp->lock, NULL);
    pthread_cond_init(&sp->empty, NULL);
    pthread_cond_init(&sp->full, NULL);
    if (sp == NULL)
    {
        printf("Error allocating memory for the stack\n");
        exit(-1);
    }
    sp->size = size;
    sp->top = -1;
    sp->sp = malloc(sizeof(*sp->sp) * sp->size); // Allocate memory for the stack_el struct
    //Individual allocation for each "stack_el array", had to cast the return value of malloc (why?);
    for (int i = 0; i < sp->size; i++)
    {
        sp->sp[i] = (struct stack_el *)malloc(sizeof(struct stack_el));
    }
    return sp;
}

int Push_Stack(struct stack *sp, struct stack_el *el)
{
    //printf("Pushing an element into the stack\n");
    pthread_mutex_lock(&sp->lock);
    while (sp->top == sp->size - 1)
    {
        printf("Stack Full\n");
        pthread_cond_wait(&sp->empty, &sp->lock);
        //pthread_mutex_unlock(&sp->lock); // Unlock here, otherwise program stuck
    }
    sp->sp[++sp->top] = el;
    pthread_cond_signal(&sp->full);
    printf("%d, %s pushed into the stack\n", el->n, el->str);
    pthread_mutex_unlock(&sp->lock);
    return 1;
}

struct stack_el *Pop_Stack(struct stack *sp)
{
    pthread_mutex_lock(&sp->lock);
    while (sp->top == -1)
    {
        printf("Stack is empty\n");
        pthread_cond_wait(&sp->full, &sp->lock);
        //pthread_mutex_unlock(&sp->lock); // Unlock here, otherwise program stuck
        //return NULL;
    }
    pthread_cond_signal(&sp->empty);
    printf("Item %d %s removed from stack\n", sp->sp[sp->top]->n, sp->sp[sp->top]->str);
    pthread_mutex_unlock(&sp->lock);
    return sp->sp[sp->top--];
}

void Show(struct stack *sp)
{
    int i;
    printf("Stack list:\n");
    pthread_mutex_lock(&sp->lock);
    for (i = 0; i <= sp->top; i++)
    {
        printf("Item: %d %s\n", sp->sp[i]->n, sp->sp[i]->str);
    }
    pthread_mutex_unlock(&sp->lock);
}

void Free_Stack(struct stack *sp)
{
    pthread_mutex_lock(&sp->lock);
    free(sp); // Is it freeing all structures ?
    sp->sp = NULL;
    sp->size = 0;
    sp->top = -1;
    pthread_mutex_unlock(&sp->lock);
}
