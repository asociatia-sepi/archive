#include <bits/stdc++.h>
#define xx first
#define yy second
#define l(x) (x << 1) + 1
#define r(x) (x << 1) + 2
using namespace std;
typedef pair<int, int> pii;
const int maxN = 1e5 + 5;
const int maxM = 2e5 + 5;

int start[maxN];
bool vis[maxN];

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
} dsu1, dsu2;

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
        node[id].mxm = {0, -l};
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

struct Update
{
    int u, v, l, r, i;
};
bool added[maxM];
int remtime[maxM], ind[maxN];

vector<Update> updates;
vector<pii> edges;
vector<int> add[maxN];
vector<int> rem[maxN];

void divide_and_conquer(int n, int l, int r, vector<Update> updates)
{
    vector<Update> filtered;
    for (auto &it : updates)
    {
        if (max(l, it.l) <= min(r, it.r))
        {
            filtered.push_back(it);
        }
    }
    filtered.swap(updates);
    dsu1.init(n);
    dsu2.init(n);
    for (auto &it : updates)
    {
        if ((it.l > l || it.r < r) && dsu1.root(it.u) != dsu1.root(it.v))
        {
            dsu1.unite(it.u, it.v);
        }
    }
    for (auto &it : updates)
    {
        if (it.l <= l && r <= it.r && dsu1.root(it.u) != dsu1.root(it.v))
        {
            dsu1.unite(it.u, it.v);
            dsu2.unite(it.u, it.v);
            added[it.i] = true;
            remtime[it.i] = max(remtime[it.i], r);
        }
    }
    int cur = 0;
    for (auto &it : updates)
    {
        ind[it.u] = 0;
        ind[it.v] = 0;
    }
    for (auto &it : updates)
    {
        if (!ind[dsu2.root(it.u)])
            ind[dsu2.root(it.u)] = ++cur;
        if (!ind[dsu2.root(it.v)])
            ind[dsu2.root(it.v)] = ++cur;
    }
    dsu1.init(cur);
    for (auto &it : updates)
    {
        it.u = ind[dsu2.root(it.u)];
        it.v = ind[dsu2.root(it.v)];
        if (it.l <= l && r <= it.r && dsu1.root(it.u) != dsu1.root(it.v))
        {
            dsu1.unite(it.u, it.v);
        }
        else if (it.l <= l && r <= it.r)
        {
            it.l = maxN;
            it.r = 0;
        }
    }
    int q = l + r >> 1;
    if (l == r)
    {
        return;
    }
    divide_and_conquer(cur, l, q, updates);
    divide_and_conquer(cur, q + 1, r, updates);
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    for (int i = 0; i < M; ++i)
    {
        X[i]++, Y[i]++;
        edges.push_back({X[i], Y[i]});
    }
    sort(edges.begin(), edges.end(), greater<pii>{});
    for (int i = 0; i < edges.size(); ++i)
    {
        updates.push_back({edges[i].xx, edges[i].yy, edges[i].yy, N, i});
    }
    divide_and_conquer(N, 1, N, updates);
    for (int i = 0; i < edges.size(); ++i)
    {
        if (added[i])
        {
            add[edges[i].yy].push_back(edges[i].xx);
            rem[remtime[i]].push_back(edges[i].xx);
        }
    }
    build(0, 1, N);
    for (int i = 1; i <= N; ++i)
    {
        update(0, i, i, i);
        for (int v : add[i])
        {
            update(0, 1, v, 1);
        }
        start[i] = -query(0, 1, i).yy;
        for (int v : rem[i])
        {
            update(0, 1, v, -1);
        }
    }
    vector<int> ans;
    for (int i = N; i; i = start[i] - 1)
    {
        ans.push_back(i - start[i] + 1);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}