#include <stdio.h>

int main(int argc, char const *argv[])
{

    int n;

    int is_prime = 1;

    scanf("%d", &n);

    if (n < 2)
    {
        is_prime = 0;
    }

    for (int i = 2; i < n / i; i++)
    {
        if (n % i == 0)
        {
            is_prime = 0;
            break;
        }
    }

    if (is_prime == 1)
    {
        printf("is_prime\n");
    }
    else
    {
        printf("is_not_prime\n");
    }

    return 0;
}
