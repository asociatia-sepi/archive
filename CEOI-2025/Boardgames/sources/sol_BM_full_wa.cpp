#include "boardgames.h"

#define MAXN 100000

std::vector<int> edge[MAXN];

int cnt, maxi;
int p[MAXN];
int where(int x)
{
    if (p[x] < 0)
        return x;
    return (p[x] = where(p[x]));
}
void uni(int x, int y)
{
    x = where(x);
    y = where(y);
    if (x == y)
        return;

    cnt--;
    if (p[x] < p[y])
    {
        p[y] = x;
    }
    else
    {
        if (p[x] == p[y])
            p[y]--;
        p[x] = y;
    }
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    std::vector<int> ans;
    ans.clear();

    for (int i = 0; i < M; i++)
    {
        edge[Y[i]].push_back(X[i]);
    }

    int a = 0;
    while (a < N)
    {
        for (int i = 0; i < N; i++)
            p[i] = -1;
        cnt = 0, maxi = 0;
        for (int i = a; i < std::min(N, a + 2000); i++)
        {
            cnt++;
            for (int j : edge[i])
            {
                if (j >= a)
                    uni(i, j);
            }
            if (cnt == 1)
                maxi = std::max(maxi, i);
        }
        ans.push_back(maxi - a + 1);
        a = maxi + 1;
    }

    return ans;
}
