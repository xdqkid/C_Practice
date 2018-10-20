#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20000
int data[MAXSIZE];
int cmp(const void *a, const void *b)
{
    return *((int*)a) - *((int*)b);
}
int main()
{
    FILE *output;
    output = fopen("input.txt", "wb+");
    if (output == NULL)
    {
        printf("FILE OPEN ERROR");
        exit(0);
    }
    int i, s;
    for (i = 0; i < MAXSIZE; ++i)
    {
        s = i % 500 + 2;
        if (s == 3 || s == 5 || s == 7)
            s *= s;
        if (s > 500)
            s = 2;
        data[i] = s;
    }
    qsort(data, MAXSIZE, sizeof(int),cmp);
    for (i = 0; i < MAXSIZE; ++i)
    {
        printf("%d\t", data[i]);
        if (i % 5 == 0)
            printf("\n");
    }
    fwrite(data, sizeof(int), MAXSIZE, output);
    fclose(output);
    return 0;
}

