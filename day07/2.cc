#include <stdio.h>
#include <stdint.h>

void print_one_intervals(uint32_t x)
{
    int previous = -1;

    for (int i = 0; i < 32; ++i)
    {
        uint32_t mask = 1u << i;

        if ((x & mask) != 0u)
        {
            if (previous >= 0)
            {
                printf("bit%d 到 bit%d：中间有%d个0\n",
                       previous,
                       i,
                       i - previous - 1);
            }

            previous = i;
        }
    }
}

int main(int argc, char const *argv[])
{
    print_one_intervals(0x1001);
    return 0;
}
