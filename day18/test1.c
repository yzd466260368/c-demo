#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 10

sem_t a, b;

void *printA(void *args)
{

    for (int i = 0; i < NUM; i++)
    {
        sem_wait(&a);
        printf("A");
        sem_post(&b);
    }
}

void *printB(void *args)
{

    for (int i = 0; i < NUM; i++)
    {
        sem_wait(&b);
        printf("B");
        sem_post(&a);
    }
}

int main(int argc, char const *argv[])
{

    pthread_t t1, t2;

    sem_init(&a, 0, 1);
    sem_init(&b, 0, 0);

    pthread_create(&t1, NULL, printA, NULL);

    pthread_create(&t2, NULL, printB, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&a);
    sem_destroy(&b);

    return 0;
}
