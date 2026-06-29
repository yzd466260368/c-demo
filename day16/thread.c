#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 10

sem_t semA, semB;

void *printfA(void *args)
{
    for (int i = 0; i < NUM; i++)
    {
        sem_wait(&semA);

        printf("A");
        fflush(stdout);

        sem_post(&semB);
    }
}

void *printfB(void *args)
{

    for (int i = 0; i < NUM; i++)
    {
        sem_wait(&semB);

        printf("B");
        fflush(stdout);

        sem_post(&semA);
    }
}

int main(int argc, char const *argv[])
{

    pthread_t threadA, threadB;

    sem_init(&semA, 0, 1); // A先执行
    sem_init(&semB, 0, 0); // B后执行

    pthread_create(&threadA, NULL, printfA, NULL);
    pthread_create(&threadB, NULL, printfB, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    sem_destroy(&semA);
    sem_destroy(&semB);

    return 0;
}

/*

线程同步常见方法有互斥锁、条件变量、信号量、读写锁、自旋锁和屏障。
两个线程交替打印 A、B，可以使用互斥锁保护共享变量 turn，再通过条件变量让不满足条件的线程睡眠。线程A只在 turn == 0 时打印，线程B只在 turn == 1 时打印，打印后修改 turn 并唤醒另一个线程。

#include <stdio.h>
#include <pthread.h>

#define PRINT_COUNT 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// 0表示轮到A，1表示轮到B
int turn = 0;

void *printA(void *arg)
{
    for (int i = 0; i < PRINT_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        // 必须使用while，防止条件变量虚假唤醒
        while (turn != 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("A");
        fflush(stdout);

        turn = 1;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *printB(void *arg)
{
    for (int i = 0; i < PRINT_COUNT; i++) {
        pthread_mutex_lock(&mutex);

        while (turn != 1) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("B");
        fflush(stdout);

        turn = 0;
        pthread_cond_signal(&cond);

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(void)
{
    pthread_t tidA;
    pthread_t tidB;

    pthread_create(&tidA, NULL, printA, NULL);
    pthread_create(&tidB, NULL, printB, NULL);

    pthread_join(tidA, NULL);
    pthread_join(tidB, NULL);

    printf("\n");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}

*/
