#include <bits/stdc++.h>
#define l(x) (x << 1) + 1
#define r(x) (x << 1) + 2
using namespace std;
const int maxN = 1e5 + 5;
const int maxK = 5 * maxN;
const int L = 20;

struct Graph
{
    int par[maxN];
    int hgt[maxN];
    int root[maxN];
    int mxm[maxN][L];
    int mnm[maxN][L];
    int anc[maxN][L];
    vector<int> adj[maxN];
} graph;

struct Digraph
{
    bool vis[maxK];
    vector<int> adj[maxK];
} digraph;

int lca(int u, int v)
{
    if (graph.root[u] != graph.root[v])
    {
        return 0;
    }
    if (graph.hgt[u] < graph.hgt[v])
    {
        swap(u, v);
    }
    int dif = graph.hgt[u] - graph.hgt[v];
    for (int i = 0; i < L; ++i)
    {
        if (dif >> i & 1)
        {
            u = graph.anc[u][i];
        }
    }
    if (u == v)
    {
        return u;
    }
    for (int i = L - 1; i >= 0; --i)
    {
        if (graph.anc[u][i] != graph.anc[v][i])
        {
            u = graph.anc[u][i];
            v = graph.anc[v][i];
        }
    }
    return graph.par[u];
}

int path_max(int u, int v)
{
    int a = lca(u, v);
    if (!a)
    {
        return maxN - 1;
    }
    auto line_max = [](int u, int a)
    {
        int dif = graph.hgt[u] - graph.hgt[a] + 1, ans = 0;
        for (int i = 0; i < L; ++i)
        {
            if (dif >> i & 1)
            {
                ans = max(ans, graph.mxm[u][i]);
                u = graph.anc[u][i];
            }
        }
        return ans;
    };
    return max(line_max(u, a), line_max(v, a));
}

int path_min(int u, int v)
{
    int a = lca(u, v);
    if (!a)
    {
        return 0;
    }
    auto line_min = [](int u, int a)
    {
        int dif = graph.hgt[u] - graph.hgt[a] + 1, ans = maxN - 1;
        for (int i = 0; i < L; ++i)
        {
            if (dif >> i & 1)
            {
                ans = min(ans, graph.mnm[u][i]);
                u = graph.anc[u][i];
            }
        }
        return ans;
    };
    return min(line_min(u, a), line_min(v, a));
}

void dfs(int u, int root)
{
    graph.root[u] = root;
    graph.anc[u][0] = graph.par[u];
    graph.mxm[u][0] = graph.mnm[u][0] = u;
    for (int i = 1; i < L; ++i)
    {
        graph.anc[u][i] = graph.anc[graph.anc[u][i - 1]][i - 1];
        graph.mxm[u][i] = max(graph.mxm[u][i - 1], graph.mxm[graph.anc[u][i - 1]][i - 1]);
        graph.mnm[u][i] = min(graph.mnm[u][i - 1], graph.mnm[graph.anc[u][i - 1]][i - 1]);
    }
    for (int v : graph.adj[u])
    {
        if (graph.par[u] != v)
        {
            graph.par[v] = u;
            graph.hgt[v] = graph.hgt[u] + 1;
            dfs(v, root);
        }
    }
}

void add_edge(int i, int l, int r, int id = 0, int L = 0, int R = maxN - 1)
{
    if (l == L && r == R)
    {
        digraph.adj[id + maxN].push_back(i);
        return;
    }
    int q = L + R >> 1;
    if (l <= q)
    {
        add_edge(i, l, min(q, r), l(id), L, q);
    }
    if (r > q)
    {
        add_edge(i, max(q + 1, l), r, r(id), q + 1, R);
    }
}

void add_intervals(int id = 0, int L = 0, int R = maxN - 1)
{
    if (L == R)
    {
        digraph.adj[L].push_back(id + maxN);
        return;
    }
    else
    {
        digraph.adj[l(id) + maxN].push_back(id + maxN);
        digraph.adj[r(id) + maxN].push_back(id + maxN);
    }
    int q = L + R >> 1;
    add_intervals(l(id), L, q);
    add_intervals(r(id), q + 1, R);
}

void dfs(int u)
{
    digraph.vis[u] = true;
    for (int v : digraph.adj[u])
    {
        if (!digraph.vis[v])
        {
            dfs(v);
        }
    }
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    for (int i = 0; i < M; ++i)
    {
        X[i]++, Y[i]++;
        graph.adj[X[i]].push_back(Y[i]);
        graph.adj[Y[i]].push_back(X[i]);
    }
    for (int i = 1; i <= N; ++i)
    {
        if (!graph.par[i])
        {
            dfs(i, i);
        }
    }
    auto process = [](int i)
    {
        int l = path_min(i, i + 1);
        int r = path_max(i, i + 1);
        add_edge(i, l, r - 1);
    };
    add_intervals();
    for (int i = 1; i < N; ++i)
    {
        process(i);
    }
    dfs(0);
    dfs(maxN - 1);

    vector<int> ans;
    int l = 1;
    for (int i = 1; i < N; ++i)
    {
        if (digraph.vis[i])
        {
            ans.push_back(i - l + 1), l = i + 1;
        }
    }
    ans.push_back(N - l + 1);
    return ans;
}