#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;
void ZeroOneKnapsack(int w, int v, int V, int *f)
{
    for (int j = V; j >= v; --j)
        f[j] = max(f[j], f[j - v] + w);
}
void CompleteKnapsack(int w, int v, int V, int *f)
{
    for (int j = v; j <= V; ++j)
        f[j] = max(f[j], f[j - v] + w);
}
int MultipleKnapsack()
{
    int n, V, *m, *w, *v, *f;
    int i, j, k, x;
    int *queue, head, tail, queue_size;
    cin >> V >> n;
    m = (int*)malloc(sizeof(int) * n);
    w = (int*)malloc(sizeof(int) * n);
    v = (int*)malloc(sizeof(int) * n);
    queue_size = 0;
    for (i = 0; i < n; ++i)
    {
        cin >> v[i] >> w[i] >> m[i];
        queue_size = max(queue_size, m[i]);
    }   
    f = (int*)malloc(sizeof(int) * (V + 1));
    queue = (int*)malloc(sizeof(int) * (queue_size + 1));
    memset(f, 0, sizeof(int) * (V + 1));
    for (i = 0; i < n; ++i)
    {
        if (m[i] == 1)
        {
            ZeroOneKnapsack(w[i], v[i], V, f);
            break;
        }
        if (m[i] * v[i] >= V)
        {
            CompleteKnapsack(w[i], v[i], V, f);
            break;
        }
        for (j = 0; j < v[i]; ++j)
        {
            head = 0, tail = -1;
            for (k = 0; j + k * v[i] <= V; ++k)
            {
                while (x > queue[tail] && tail >= head)
                    --tail;
                queue[++tail] = x;   
                if (head + m[i] == tail)
                    ++head;
                x = f[j + k * v[i]] - k * w[i];
                f[j + k * v[i]] = queue[head] + k * w[i];
            }
        }
    }
    cout << f[V] << endl;
    return 0;
}
int main(void)
{
    MultipleKnapsack();
    return 0;
}