#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Path
{
	char ori[4];
	char des[4];
}Path;
typedef struct Request
{
	char ori[4];
	char des[4];
	int max;
}Request;
Path path[20];
Request req[20];
int sum_path, sum_req;
void readPath(FILE *pi, Path *p)
{
    char str[20];
    fgets(str, 20, pi);
    strncpy(p->ori, str + 1, 3);
    strncpy(p->des, str + 6, 3);
}
void readRequest(FILE *pr, Request *p)
{
    char str[20];
    fgets(str, 20, pr);
    strncpy(p->ori, str + 1, 3);
    strncpy(p->des, str + 6, 3);
    sscanf(str + 10, "%d", &p->max);
}
//从当前节点向子节点出发，深度搜索max层
int dfs(char *ori, char *des, int max)
{
    int result, i;
    result = 0;
    for (i = 0; i < sum_path; ++i)
    {
        if (strcmp(ori, path[i].ori) == 0)
        {
            if (strcmp(des, path[i].des) == 0)
                return 1;
            if (max > 1 && dfs(path[i].des, des, max - 1) == 1)
            {
                return 1;
            }
        }
    }
    return 0;
}
int main()
{
    int i, result;
    FILE *pi, *pr, *out;
    pi = fopen("PathInput.txt", "r");
    pr = fopen("PathRequest.txt", "r");
    out = fopen("Output.txt", "w");
    if (pi == NULL || pr == NULL || out == NULL)
    {
        printf("FILE OPEN ERROR");
        return 0;
    }
    fscanf(pi, "%d\n", &sum_path);
    for (i = 0; i < sum_path; ++i)
        readPath(pi, &path[i]);
    fscanf(pr, "%d\n", &sum_req);
    for (i = 0; i < sum_req; ++i)
        readRequest(pr, &req[i]);
    for (i = 0; i < sum_req; ++i)
    {
        fprintf(out, "[%s, %s, %s]\n", req[i].ori, req[i].des, dfs(req[i].ori, req[i].des, req[i].max)?"YES\0":"NO\0");
    }
    fclose(out);
    fclose(pr);
    fclose(pi);
    return 0;
}

