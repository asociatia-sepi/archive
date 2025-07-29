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
    void init(int l, int r)
    {
        for (int i = l; i <= r; ++i)
        {
            par[i] = i;
        }
    }
} dsu;

vector<int> adj[maxN];
vector<int> ans;

void divide_and_conquer(int l, int r)
{
    dsu.init(l, r);
    for (int i = l; i < r; ++i)
    {
        for (int v : adj[i])
        {
            if (l <= v && v <= r && dsu.root(v) != dsu.root(i))
            {
                dsu.unite(v, i);
            }
        }
    }
    int q = l + r >> 1;
    for (int i = 0; q - i >= l || q + i < r; ++i)
    {
        auto check = [&](int p)
        {
            if (p < l || p >= r)
            {
                return false;
            }
            if (dsu.root(p) != dsu.root(p + 1))
            {
                divide_and_conquer(l, p);
                divide_and_conquer(p + 1, r);
                return true;
            }
            return false;
        };
        if (check(q - i))
            return;
        if (check(q + i))
            return;
    }
    ans.push_back(r - l + 1);
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    for (int i = 0; i < M; ++i)
    {
        adj[X[i]].push_back(Y[i]);
        adj[Y[i]].push_back(X[i]);
    }
    divide_and_conquer(0, N - 1);
    return ans;
}