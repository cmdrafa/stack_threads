#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack_c.h"

int thread_global;

void *thread_call(void *arg)
{
    struct stack *stack_main = (struct stack *)arg;

    for (int i = 0; i <= (thread_global + 1); i++)
    {
        if (stack_main->sp[i]->str != NULL)
        {
            Push_Stack(stack_main, stack_main->sp[i]);
        }
        //Pop_Stack(stack_main);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("ERROR!\nYou must pass the size of the stack and the number of threads.\n");
        exit(1);
    }
    int size = atoi(argv[1]);
    int n_threads = atoi(argv[2]);
    if ((n_threads + 3 > size))
    {
        printf("ERROR!\nThe number of threads is too big (or stack size too small).\nIncrease the stack size or reduce the number of threads.\n");
        exit(1);
    }
    thread_global = n_threads;

    struct stack *stack_main = Init_Stack(size);
    pthread_t p[n_threads];

    for (int i = 0; i < n_threads; i++)
    {
        stack_main->sp[i]->n = i + 1;
        stack_main->sp[i]->str = "Random string";
        int rc = pthread_create(&p[i], NULL, thread_call, stack_main);
        if (rc < 0)
        {
            perror("Thread create");
        }
    }

    for (int i = 0; i < n_threads; i++)
    {
        int rc = pthread_join(p[i], NULL);
        if (rc < 0)
        {
            perror("Thread join");
        }
    }

    Show(stack_main);
    Free_Stack(stack_main);

    return 0;
}