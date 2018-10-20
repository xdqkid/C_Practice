#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;
int solve_2D()
{
    int n, V, *w, *v, **f;
    int i, j, k, maxf;
    cin >> V >> n;
    w = (int*)malloc(sizeof(int) * (n + 1));
    v = (int*)malloc(sizeof(int) * (n + 1));
    for (i = 1; i <= n; ++i)
        cin >> v[i] >> w[i];
    f = (int**)malloc(sizeof(int*) * (V + 1));
    for (i = 0; i <= V; ++i)
    {
        f[i] = (int*)malloc(sizeof(int) * (V + 1));
        memset(f[i], 0, sizeof(int) * (V + 1));
    }   
    for (i = 1; i <= n; ++i)
        for (j = 0; j <= V; ++j)
        {
            k = 1;
            maxf = f[i - 1][j];
            while (j - v[i] * k >= 0)
            {
                if (maxf < f[i - 1][j - v[i] * k] + w[i])
                    maxf = f[i - 1][j - v[i] * k] + w[i];
                ++k;
            }
            f[i][j] = maxf;
        }

    cout << f[n][V] << endl;
    return 0;
}
int solve_1D()
{
    int n, V, *w, *v, *f;
    int i, j;
    cin >> V >> n;
    w = (int*)malloc(sizeof(int) * (n + 1));
    v = (int*)malloc(sizeof(int) * (n + 1));
    for (i = 1; i <= n; ++i)
        cin >> v[i] >> w[i];
    f = (int*)malloc(sizeof(int) * (V + 1));
    memset(f, 0, sizeof(int) * (V + 1));
    for (i = 0; i <= V; ++i)
        f[i] = 0;
    for (i = 1; i <= n; ++i)
        for (j = v[i]; j <= V; ++j)
            f[j] = max(f[j], f[j - v[i]] + w[i]);
    cout << f[V] << endl;
}
int main(void)
{
    solve_1D();
    solve_2D();
    return 0;
}