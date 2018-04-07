#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20000
int data[20000];
int maxset[MAXSIZE], len;
int gcd(int a, int b) //求最大公约数
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
int cmp(const void *a, const void *b)
{
    return *((int*)a) - *((int*)b);
}
void find_maxset()
{
    int i, j;
    len = 0;
    for (i = 0; i < MAXSIZE; ++i)
    {
        for (j = 0; j < len; ++j)
            if (gcd(data[i], maxset[j]) != 1)//暴力枚举，因已排序，速度实际很快
            {
                break;
            }
        if (j == len)//与子集所有元素没有除1以外的公约数
            maxset[len++] = data[i];
    }
}
void print_answer(FILE *output)
{
    int i, j;
    printf("%d\n", len);
    fprintf(output, "%d\n", len);

    for (i = 0, j = 0; i < len; ++i)
    {
        printf("%d", maxset[i]);
        fprintf(output, "%d", maxset[i]);

        ++j;
        if (j % 5 == 0)
        {
            printf("\n");
            fprintf(output, "\n");

            j = 0;
        }
        else
        {
            printf("\t");
            fprintf(output, "\n");
        }
    }
}
int main()
{

    int i, j, flag;

    FILE *input, *output;
    input = fopen("input.txt", "rb");
    output = fopen("output.txt", "w");
    if (input == NULL || output == NULL)
    {
        printf("FILE OPEN ERROR");
        exit(0);
    }

    fread(data, sizeof(int), MAXSIZE, input); //注意：参数顺序，fread/fwrite参数顺序和qsort顺序不同
    qsort(data, MAXSIZE, sizeof(int), cmp);//注意：这里偷懒了，题目要求不准用函数库。因输入集合已排序，排不排序均可。
    find_maxset();
    print_answer(output);

    fclose(output);
    fclose(input);

    return 0;
}
