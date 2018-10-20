#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct Word
{
    char c[20];
    int count;
    struct Word *next;
}Word;
int insert(Word *list, char *w)
{
    Word *p;
    p = list->next;
    while (p != NULL)
    {
        if (strcmp(p->c, w) == 0)
        {
            ++(p->count);
            return 1;
        }
        p = p->next;
    }
    p = (Word*)malloc(sizeof(Word));
    if (p == NULL)
        return 0;
    //ͷ�巨
    strcpy(p->c, w);
    p->count = 1;
    p->next = list->next;
    list->next = p;
    return 1;
}
void sort2(Word* list)
{
    Word *p, *q, *rear, *max, *pre;
    rear = list;
    p = list->next;
    while (p != NULL)
    {
        max = p;
        pre = p;
        q = p;
        while (q->next != NULL)
        {
            if (q->next->count == max->count && strcmp(q->next->c, max->c) < 0 || q->next->count > max->count)
            {
                pre = q;
                max = q->next;
            }
            q = q->next;
        }
        if (max != p) {
            pre->next = max->next;
            rear->next = max;
            rear = max;
            rear->next = NULL;
        }
        else {
            rear->next = p;
            p = p->next;
            rear = rear->next;
            rear->next = NULL;
        }
    }

}
void printList(Word* list)
{
    Word* p;
    p = list->next;
    while (p != NULL)
    {
        printf("%s: %d\n", p->c, p->count);
        p = p->next;
    }
    printf("====\n");
}
int main()
{
    FILE *fpi,*fpo;
    const char *input = "C:\\Users\\Nick\\Documents\\input.txt";
    const char *output = "C:\\Users\\Nick\\Documents\\output.txt";
    Word *list;
    list = (Word*)malloc(sizeof(Word));
    list->next = NULL;
    char buff[1024];

    fpi = fopen(input, "r");
    if (fpi == NULL)
    {
        printf("Open Error");
        exit(0);
    }
    fpo = fopen(output, "w+");
    if (fpo == NULL)
    {
        printf("Open Error2");
        exit(0);
    }
    rewind(fpi);

    while (!feof(fpi))
    {
        if (fscanf(fpi, "%s", buff))
            insert(list, buff);
    }
    sort2(list);
    printList(list);


    Word* p;
    p = list->next;
    while (p != NULL)
    {
        fprintf(fpo, "%s: %d\n", p->c, p->count);
        p = p->next;
    }



    fclose(fpi);
    fclose(fpo);

    return 0;
}


void sort(Word* list)
{
    Word *p, *q, *max;
    int t;
    char ct[20];
    for (p = list->next; p->next != NULL; p = p->next)
    {
        max = p;
        for (q = p->next; q->next != NULL; q = q->next)
            if (q->count == max->count && strcmp(q->c, max->c) < 0 || q->count > max->count)
                max = q;
        if (max != p)
        {
            t = max->count;
            max->count = p->count;
            p->count = t;

            strcpy(ct, max->c);
            strcpy(max->c, p->c);
            strcpy(p->c, ct);
        }
    }

}

