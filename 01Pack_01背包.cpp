#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;
int solve_2D()
{
    int n, V, *w, *v, **f;
    int i, j;
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
        for (j = V; j >= 0; --j)
            f[i][j] = j - v[i] >= 0 ?
                max(f[i - 1][j], f[i - 1][j - v[i]] + w[i]) 
                : f[i - 1][j];
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
    for (i = 1; i <= n; ++i)
        for (j = V; j >= v[i]; --j)
            f[j] = max(f[j], f[j - v[i]] + w[i]);
    cout << f[V] << endl;
}
int main(void)
{
    // solve_1D();
    solve_2D();
    return 0;
}