#include <bits/stdc++.h>
#define xx first
#define yy second
#define l(x) (x << 1) + 1
#define r(x) (x << 1) + 2
using namespace std;
typedef pair<int, int> pii;

const int maxN = 1e5 + 5;
int endp[maxN];
bool vis[maxN];

vector<int> adj[maxN];

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
        for (int i = 1; i <= n; ++i)
        {
            par[i] = i;
            cnt[i] = 1;
        }
    }
} dsu;

struct Node
{
    int l, r, lzy;
    pii mxm;
} node[maxN << 2];

void build(int id, int l, int r)
{
    node[id].l = l;
    node[id].r = r;
    if (l == r)
    {
        node[id].mxm = {0, l};
        return;
    }
    int q = l + r >> 1;
    build(l(id), l, q);
    build(r(id), q + 1, r);
}

void lazy(int id)
{
    int lzy = node[id].lzy;
    if (node[id].l != node[id].r)
    {
        node[l(id)].lzy += lzy;
        node[r(id)].lzy += lzy;
    }
    node[id].mxm.xx += lzy;
    node[id].lzy = 0;
}

void update(int id, int l, int r, int x)
{
    lazy(id);
    if (node[id].l == l && node[id].r == r)
    {
        node[id].lzy += x;
        return;
    }
    int q = node[id].l + node[id].r >> 1;
    if (l <= q)
    {
        update(l(id), l, min(q, r), x);
    }
    if (r > q)
    {
        update(r(id), max(q + 1, l), r, x);
    }
    lazy(l(id)), lazy(r(id));
    node[id].mxm = max(node[l(id)].mxm, node[r(id)].mxm);
}

pii query(int id, int l, int r)
{
    lazy(id);
    if (node[id].l == l && node[id].r == r)
    {
        return node[id].mxm;
    }
    int q = node[id].l + node[id].r >> 1;
    pii res = {0, 0};
    if (l <= q)
    {
        res = max(res, query(l(id), l, min(q, r)));
    }
    if (r > q)
    {
        res = max(res, query(r(id), max(q + 1, l), r));
    }
    return res;
}

struct Graph
{
    int par[maxN];
    int wgt[maxN];
    bool stk[maxN];
    set<int> adj[maxN];
} graph;

struct Edge
{
    int u, w, root;

    bool operator<(const Edge &e) const
    {
        return w < e.w;
    }
    bool operator>(const Edge &e) const
    {
        return w > e.w;
    }
};

Edge max_edge(int a, int b)
{
    int anca, ancb;

    vector<int> anc;
    Edge res = {0, 0, 0};

    for (anca = a; anca != 0; anca = graph.par[anca])
    {
        anc.push_back(anca);
        vis[anca] = true;
    }
    for (ancb = b; !vis[ancb]; ancb = graph.par[ancb])
    {
        res = max(res, {ancb, graph.wgt[ancb], b});
    }
    for (anca = a; anca != ancb; anca = graph.par[anca])
    {
        res = max(res, {anca, graph.wgt[anca], a});
    }
    for (int u : anc)
    {
        vis[u] = false;
    }
    return res;
}

void dfs(int u, int p)
{
    graph.stk[u] = true;
    graph.par[u] = p;
    graph.wgt[u] = max(u, p);
    for (int v : graph.adj[u])
    {
        if (!graph.stk[v])
        {
            dfs(v, u);
        }
    }
    graph.stk[u] = false;
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    for (int i = 0; i < M; ++i)
    {
        X[i]++, Y[i]++;
        adj[X[i]].push_back(Y[i]);
    }
    dsu.init(N);
    build(0, 1, N);

    for (int i = N; i >= 1; --i)
    {
        update(0, i, i, N - i);
        for (int v : adj[i])
        {
            if (dsu.root(i) != dsu.root(v))
            {
                int a = i, cnta = dsu.cnt[dsu.root(a)];
                int b = v, cntb = dsu.cnt[dsu.root(b)];
                dsu.unite(a, b);

                if (cnta > cntb)
                {
                    swap(a, b);
                }
                dfs(a, b);
                graph.adj[a].insert(b);
                graph.adj[b].insert(a);
                update(0, v, N, 1);
            }
            else
            {
                int a = i, b = v;
                Edge res = max_edge(a, b);

                if (res.w <= v)
                {
                    continue;
                }
                if (res.root != a)
                {
                    swap(a, b);
                }
                graph.adj[res.u].erase(graph.par[res.u]);
                graph.adj[graph.par[res.u]].erase(res.u);
                graph.adj[a].insert(b);
                graph.adj[b].insert(a);

                vector<pii> parChange;
                vector<pii> wgtChange;
                for (int u = a, p = b, w = v;; p = u, u = graph.par[u], w = graph.wgt[p])
                {
                    parChange.push_back({u, p});
                    wgtChange.push_back({u, w});
                    if (u == res.u)
                    {
                        break;
                    }
                }
                for (pii p : parChange)
                {
                    graph.par[p.xx] = p.yy;
                }
                for (pii p : wgtChange)
                {
                    graph.wgt[p.xx] = p.yy;
                }
                update(0, v, res.w - 1, 1);
            }
        }
        endp[i] = query(0, i, N).yy;
    }
    vector<int> ans;
    for (int i = 1; i <= N; i = endp[i] + 1)
    {
        ans.push_back(endp[i] - i + 1);
    }
    return ans;
}