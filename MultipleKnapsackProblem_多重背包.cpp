#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;
int ZeroOneKnapsack(int w, int v, int V, int *f)
{
    for (int j = V; j >= v; --j)
        f[j] = max(f[j], f[j - v] + w);
    return f[V];
}
int CompleteKnapsack(int w, int v, int V, int *f)
{
    for (int j = v; j <= v; ++j)
        f[j] = max(f[j], f[j - v] + w);
    return f[V];
}
int MultipleKnapsack()
{
    int n, V, *m, *w, *v, *f;
    int i, j, k;
    cin >> V >> n;
    m = (int*)malloc(sizeof(int) * (n + 1));
    w = (int*)malloc(sizeof(int) * (n + 1));
    v = (int*)malloc(sizeof(int) * (n + 1));
    for (i = 0; i < n; ++i)
        cin >> v[i] >> w[i] >> m[i];
    f = (int*)malloc(sizeof(int) * (V + 1));
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
        k = 1;
        while (k < m[i])
        {
            ZeroOneKnapsack(k * w[i], k * v[i], V, f);
            m[i] -= k;
            k *= 2;
        }
        ZeroOneKnapsack(w[i] * m[i], v[i] * m[i], V, f);
    }
    cout << f[V] << endl;
    return 0;
}
int main(void)
{
    MultipleKnapsack();
    return 0;
}