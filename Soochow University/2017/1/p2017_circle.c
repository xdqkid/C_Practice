#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14
typedef struct Coordinate
{
    int x;
    int y;
} Coordinate;
typedef struct Circle
{
    Coordinate point;
    double r;
    int points_num;
    double density;
} Circle;
double distance(Coordinate a, Coordinate b)
{
    return sqrt((double)
        (a.x - b.x) * (a.x - b.x) +
        (a.y - b.y) * (a.y - b.y)
    );
}
double area(Circle c)
{
    return (c.r * c.r) * PI;
}
int draw_circles(int *data, int count, Circle **circles)
{
    int circles_num = count - 1;
    *circles = (Circle*)malloc(sizeof(Circle) * circles_num);
    for (int i = 0; i < circles_num; ++i)
    {
        (*circles)[i].point.x = data[i];
        (*circles)[i].point.y = data[i + 1];
        (*circles)[i].points_num = 0;
    }
    for (int i = 0; i < circles_num; ++i)
        (*circles)[i].r = distance((*circles)[i].point, (*circles)[(i + 1) % circles_num].point);
    for (int i = 0; i < circles_num; ++i)
        for (int j = 0; j < circles_num; ++j)
            if (distance((*circles)[i].point, (*circles)[j].point) <= (*circles)[i].r)
                (*circles)[i].points_num++;
    for (int i = 0; i < circles_num; ++i)
        (*circles)[i].density = (*circles)[i].points_num / area((*circles)[i]);
    return circles_num;
}
int read_file(const char *filename, const char *mode, int **data)
{
    FILE *fp = fopen(filename, mode);
    int count = 0;
    if (!fp)
    {
        printf("FILE OPEN ERROR\n");
        exit(-1);
    }
    fseek(fp, 0, SEEK_END);
    count = ftell(fp) / sizeof(int);
    *data = (int *)malloc(sizeof(int) * count);
    rewind(fp);
    fread(*data, sizeof(int), count, fp);
    fclose(fp);
    return count;
}
int cmp(const void *a, const void *b)
{
    double val = ((Circle*)a)->density - ((Circle*)b)->density;
    if (fabs(val) < 1e-5) //浮点型数值计算会有误差
        return 0;
    return val > 0 ? -1 : 1;
}
void print_answer(int *data, int count,Circle *circles, int num)
{
    printf("data:\n");
    for (int i = 0; i < count - 1; i++)
        printf("%d ", data[i]);
    printf("%d\n", data[count - 1]);

    printf("result:\n");
    for (int i = 0; i < num; ++i)
        printf("(%d, %d) %5d %7.2f\n", circles[i].point.x, circles[i].point.y, circles[i].points_num, circles[i].density);
}
int main()
{
    int *data = NULL;
    int count = 0;
    int circles_num = 0;
    Circle *circles = NULL;
    count = read_file("data.bin", "rb", &data);
    circles_num = draw_circles(data, count, &circles);
    qsort(circles, circles_num, sizeof(Circle), cmp);
    print_answer(data, count, circles, 5);
    return 0;    
}