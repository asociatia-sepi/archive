#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

struct DSU
{
    std::vector<int> p, h;
    std::vector<std::pair<int, bool>> upds;
    std::vector<int> edges, cnt;

    void init(int n, int m)
    {
        p.assign(n, -1);
        h.assign(n, 0);
        cnt.assign(m, 0);
        upds.clear();
        edges.clear();
    }

    int get(int x) { return p[x] == -1 ? x : get(p[x]); }

    int unite(int x, int y, int ind)
    {
        edges.push_back(ind);
        if (ind < 0 || ind >= (int)cnt.size() || ++cnt[ind] < 2)
        {
            upds.emplace_back(-1, false);
            return -1;
        }
        x = get(x), y = get(y);
        if (x == y)
        {
            upds.emplace_back(-1, false);
            return -1;
        }
        if (h[x] < h[y])
            std::swap(x, y);
        p[y] = x;
        upds.emplace_back(y, h[x] == h[y]);
        if (h[x] == h[y])
            h[x]++;
        return (int)upds.size() - 1;
    }

    void roll_back(int n = 1)
    {
        while (n && !upds.empty())
        {
            if (upds.back().first != -1)
            {
                if (upds.back().second)
                    h[p[upds.back().first]]--;
                p[upds.back().first] = -1;
            }
            if (0 <= edges.back() && edges.back() < (int)cnt.size())
            {
                cnt[edges.back()]--;
            }
            upds.pop_back();
            edges.pop_back();
            --n;
        }
    }
};

struct TrickyDSU
{
    const std::vector<std::pair<int, int>> &edges;
    const std::vector<int> &edgeidx;
    int M, L, R, nodeL, edgeL;
    DSU dsu;

    TrickyDSU(const std::vector<std::pair<int, int>> &edges_, const std::vector<int> &edgeind_, int nodeL_, int edgeL_, int n, int m) : edges(edges_), edgeidx(edgeind_), nodeL(nodeL_), edgeL(edgeL_)
    {
        dsu.init(n, m);
        build(0, m);
    }

    void build(int l, int r)
    {
        M = (l + r) / 2;
        L = M - l;
        R = r - M;
        dsu.roll_back(1e9);
        rebuild(20);
    }

    void add_edges(int l, int r)
    {
        for (int i = edgeL + l; i < edgeL + r; i++)
        {
            dsu.unite(edges[i].first - nodeL, edges[i].second - nodeL, edgeidx[i] - edgeL);
        }
    }

    void rebuild(int b)
    {
        for (int i = b; i >= 0; i--)
        {
            if (R & (1 << i))
            {
                int r = R & (~((1 << (i + 1)) - 1));
                add_edges(M + r, M + r + (1 << i));
            }
            if (L & (1 << i))
            {
                int l = L & (~((1 << (i + 1)) - 1));
                add_edges(M - l - (1 << i), M - l);
            }
        }
    }

    void remove(int b)
    {
        for (int i = 0; i <= b; i++)
        {
            if (L & (1 << i))
            {
                dsu.roll_back(1 << i);
            }
            if (R & (1 << i))
            {
                dsu.roll_back(1 << i);
            }
        }
    }

    bool same_group(int x, int y)
    {
        return dsu.get(x - nodeL) == dsu.get(y - nodeL);
    }
    int get_bit(int n, int rem)
    {
        int b = 0, m = 0;
        while (m < rem)
        {
            m |= (1 << b) & n;
            if (m >= rem)
                break;
            b++;
        }
        return b;
    }
    void delete_left(int n)
    {
        if (L > n)
        {
            int b = get_bit(L, n);
            remove(b);
            L -= n;
            rebuild(b);
        }
        else
        {
            build(M - L + n, M + R);
        }
    }
    void delete_right(int n)
    {
        if (R > n)
        {
            int b = get_bit(R, n);
            remove(b);
            R -= n;
            rebuild(b);
        }
        else
        {
            build(M - L, M + R - n);
        }
    }
};

std::vector<std::vector<int>> g;
std::vector<std::pair<int, int>> edges;
std::vector<int> edgeidx, ans, indl, indr;

TrickyDSU build(int l, int r)
{
    return TrickyDSU(edges, edgeidx, l, indl[l], r - l + 1, indr[r] - indl[l]);
}

void solve(TrickyDSU &big, int L, int R)
{
    if (L == R)
    {
        ans.push_back(1);
        return;
    }
    for (int i = 0; i <= R - L - i + 1; i++)
    {
        if (!big.same_group(L, L + i))
        {
            TrickyDSU small = build(L, L + i - 1);
            solve(small, L, L + i - 1);
            big.delete_left(indr[L + i - 1] - indl[L]);
            solve(big, L + i, R);
            return;
        }
        else if (!big.same_group(R - i, R))
        {
            big.delete_right(indr[R] - indl[R - i + 1]);
            solve(big, L, R - i);
            TrickyDSU small = build(R - i + 1, R);
            solve(small, R - i + 1, R);
            return;
        }
    }
    ans.push_back(R - L + 1);
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    g.resize(N);
    indl.resize(N);
    indr.resize(N);
    ans.clear();
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    std::map<std::pair<int, int>, int> mp;
    for (int i = 0; i < N; i++)
    {
        indl[i] = (int)edges.size();
        for (int x : g[i])
        {
            if (x < i)
            {
                edgeidx.push_back(mp[{x, i}]);
            }
            else
            {
                edgeidx.push_back(edges.size());
                mp[{i, x}] = edges.size();
            }
            edges.emplace_back(i, x);
        }
        indr[i] = (int)edges.size();
    }

    TrickyDSU big = build(0, N - 1);
    solve(big, 0, N - 1);

    return ans;
}