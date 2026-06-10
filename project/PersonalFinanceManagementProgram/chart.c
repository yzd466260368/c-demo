#include <stdio.h>
#include "chart.h"

void printBarChart(char name[], double amount)
{
    int count = (int)(amount / 50);

    if (count == 0 && amount > 0)
        count = 1;

    printf("%-10s | ", name);

    for (int i = 0; i < count; i++)
    {
        printf("*");
    }

    printf(" %.2f\n", amount);
}