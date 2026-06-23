#include <stdio.h>
#include <pthread.h>

void *thread1(void *value)
{
    printf("running\n");
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t th1;
    // WINPTHREAD_API int       pthread_create(pthread_t *th, const pthread_attr_t *attr, void *(* func)(void *), void *arg);
    int ret = 0;
    if (0 != (ret = pthread_create(&th1, NULL, &thread1, NULL)))
    {
        printf("fail\n");
        return -1;
    }

    return 0;
}
