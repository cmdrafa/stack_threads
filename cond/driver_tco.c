#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stackco_c.h"

int thread_global;

void *filler_thread(void *arg)
{
    struct stack *stack_main = (struct stack *)arg;
    long loops = 10000;
    int rnd;

    for (int i = 0; i <= (thread_global + 1); i++)
    {
        if (stack_main->sp[i]->str != NULL)
        {
            for (int j = 0; j < loops; j++)
            {
                rnd = rand() % 100 + 1;
                stack_main->sp[i]->n = stack_main->sp[i]->n + rnd;
                Push_Stack(stack_main, stack_main->sp[i]);
            }
        }
    }
    return NULL;
}

void *remover_thread(void *arg)
{
    struct stack *stack_main = (struct stack *)arg;
    long loops = 10001;

    for (int i = 0; i < loops; i++)
    {
        Pop_Stack(stack_main);
    }
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
    pthread_t f[n_threads], r[n_threads];

    for (int i = 0; i < n_threads; i++)
    {
        stack_main->sp[i]->n = i + 1;
        stack_main->sp[i]->str = "Some string";
        int rc_1 = pthread_create(&f[i], NULL, filler_thread, stack_main);
        if (rc_1 < 0)
        {
            perror("Creating filler threads");
        }
        int rc_2 = pthread_create(&r[i], NULL, remover_thread, stack_main);
        if (rc_2 < 0)
        {
            perror("Creating remover threads");
        }
    }

    for (int i = 0; i < n_threads; i++)
    {
        int rc_1 = pthread_join(f[i], NULL);
        if (rc_1 < 0)
        {
            perror("Joining filler threads");
        }
        int rc_2 = pthread_join(r[i], NULL);
        if (rc_2 < 0)
        {
            perror("Joining remover threads");
        }
    }

    Show(stack_main);
    Free_Stack(stack_main);

    return 0;
}