#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20000
int data[20000];
int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
int cmp(const void *a, const void *b)
{
    return *((int*)a) - *((int*)b);
}
int main()
{
    int maxset[MAXSIZE], len;
    int i, j, flag;
    for (i = 0; i < MAXSIZE; ++i)
        data[i] = i % 500 + 2;
    qsort(data, MAXSIZE, sizeof(int), cmp);
    len = 0;
    for (i = 0; i < MAXSIZE; ++i)
    {
        for (j = 0; j < len; ++j)
            if (gcd(data[i], maxset[j]) != 1) //使用约数集判断合会更快
            {
                break;
            }
        if (j == len)
            maxset[len++] = data[i];
    }
    printf("len = %d\n", len);
    for (i = 0, j = 0; i < len; ++i)
    {
        printf("%d", maxset[i]);
        ++j;
        if (j % 5 == 0)
        {
            printf("\n");
            j = 0;
        }
        else
            printf("\t");
    }
    return 0;
}
