#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;
const int maxN = 1e5 + 1;

struct Dsu
{
    int par[maxN];
    int cnt[maxN];

    int root(int u)
    {
        return par[u] == u ? u : par[u] = root(par[u]);
    }
    void unite(int u, int v)
    {
        u = root(u);
        v = root(v);
        if (u == v)
        {
            return;
        }
        if (cnt[u] < cnt[v])
        {
            swap(u, v);
        }
        cnt[u] += cnt[v];
        par[v] = u;
    }
    void init(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            par[i] = i;
        }
    }
} dsu;

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    dsu.init(N);
    for (int i = 0; i < M; ++i)
    {
        dsu.unite(X[i], Y[i]);
    }
    vector<int> ans;
    int l = 0;
    for (int i = 1; i < N; ++i)
    {
        if (dsu.root(i - 1) != dsu.root(i))
        {
            ans.push_back(i - l), l = i;
        }
    }
    ans.push_back(N - l);
    return ans;
}