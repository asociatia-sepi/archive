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

vector<int> adj[maxN];

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    for (int i = 0; i < M; ++i)
    {
        adj[X[i]].push_back(Y[i]);
        adj[Y[i]].push_back(X[i]);
    }
    vector<int> ans;
    for (int i = 0, next = 0; i < N; i = next)
    {
        dsu.init(N);
        next = i + 1;
        for (int j = i + 1, ccount = 2; j < N; ++j, ++ccount)
        {
            for (int v : adj[j])
            {
                if (i <= v && v <= j && dsu.root(j) != dsu.root(v))
                {
                    dsu.unite(j, v);
                    ccount--;
                }
            }
            if (ccount == 1)
            {
                next = j + 1;
            }
        }
        ans.push_back(next - i);
    }
    return ans;
}