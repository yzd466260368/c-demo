#include <stdio.h>
#include <pthread.h>

#define COUNT 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int turn = 0;

void *runA(void *arg)
{
    for (int i = 0; i < COUNT; i++)
    {
        pthread_mutex_lock(&mutex);
        while (turn != 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("A");
        fflush(stdout);

        turn = 1;

        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }
}

void *runB(void *arg)
{
    for (int i = 0; i < COUNT; i++)
    {
        pthread_mutex_lock(&mutex);
        while (turn != 1)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("B");
        fflush(stdout);

        turn = 0;

        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char const *argv[])
{

    pthread_t threadA;
    pthread_t threadB;

    pthread_create(&threadA, NULL, runA, NULL);
    pthread_create(&threadB, NULL, runB, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
