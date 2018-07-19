#include <stdio.h>
int main(int argc, char const *argv[])
{
    FILE *output = fopen("data.bin", "wb");
    if (output == NULL)
    {
        printf("error");
        return 1;
    }
    int data[] = {2,88,59,83,87,65,38,72,70,76,50,62,4,76,68,70,50,60,13,74,66,60,8,28,97,94,99,52,6,90,69,60,54,83,76,89,64,73,48,69,83,28,84,67,14,50,99,86,35,36,5,82,67,36,92,99,44,27,53,76,24,45,27,19,14,65,86,69,47,80,96,96,10,68,60,91,87,25,15,50,8,18,3,15,85,88,14,8,2,64,63,62,70,58,62,93,51,66,62,73,75,6};
    fwrite(data, sizeof(int), sizeof(data)/sizeof(int), output);
    fclose(output);
    return 0;
}
 