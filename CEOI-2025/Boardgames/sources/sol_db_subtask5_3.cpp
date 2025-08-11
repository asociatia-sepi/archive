#include <bits/stdc++.h>
#define xx first
#define yy second
#define l(x) (x << 1) + 1
#define r(x) (x << 1) + 2
using namespace std;
typedef pair<int, int> pii;

const int maxN = 1e5 + 5;
int start[maxN];

vector<int> adj[maxN];

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

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    for (int i = 0; i < M; ++i)
    {
        X[i]++, Y[i]++;
        adj[Y[i]].push_back(X[i]);
    }
    build(0, 1, N);

    for (int i = 1; i <= N; ++i)
    {
        update(0, i, i, i);
        for (int v : adj[i])
        {
            update(0, 1, v, 1);
        }
        start[i] = -query(0, 1, i).yy;
    }
    vector<int> ans;
    for (int i = N; i; i = start[i] - 1)
    {
        ans.push_back(i - start[i] + 1);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}