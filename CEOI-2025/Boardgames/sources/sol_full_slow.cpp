#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

struct DSU
{
    std::vector<int> p, h;
    std::map<std::pair<int, int>, int> cnt;
    std::vector<std::pair<int, bool>> upds;
    std::vector<std::pair<int, int>> edges;
    int shift;

    void init(int n, int s)
    {
        shift = s;
        p.assign(n, -1);
        h.assign(n, 0);
        upds.clear();
        edges.clear();
        cnt.clear();
    }
    int getp(int x) { return get(x - shift) + shift; }
    int unite(int x, int y)
    {
        x -= shift, y -= shift;
        edges.emplace_back(x, y);
        if (++cnt[{x, y}] < 2)
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
            cnt[edges.back()]--;
            upds.pop_back();
            edges.pop_back();
            --n;
        }
    }

private:
    int get(int x) { return p[x] == -1 ? x : get(p[x]); }
};

struct TrickyDSU
{
    std::vector<std::pair<int, int>> edges;
    DSU dsu;
    int M, L, R;

    TrickyDSU(const std::vector<std::pair<int, int>> &edges_, int n, int shift) : edges(edges_)
    {
        dsu.init(n, shift);
        build(0, (int)edges.size());
    }

    void build(int l, int r)
    {
        M = (l + r) / 2;
        L = M - l;
        R = r - M;
        dsu.roll_back(1e9);
        rebuild_bit(20);
    }

    void add_edges(int l, int r)
    {
        for (int i = l; i < r; i++)
        {
            dsu.unite(edges[i].first, edges[i].second);
        }
    }

    void rebuild_bit(int b)
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
        return dsu.getp(x) == dsu.getp(y);
    }
    void delete_left(int n)
    {
        if (L > n)
        {
            int b = 0, m = 0;
            while (m < n)
            {
                m |= (1 << b) & L;
                if (m >= n)
                    break;
                b++;
            }
            remove(b);
            L -= n;
            rebuild_bit(b);
        }
        else
        {
            int l = M - L, r = M + R;
            l += n;
            build(l, r);
        }
    }
    void delete_right(int n)
    {
        if (R > n)
        {
            int b = 0, m = 0;
            while (m < n)
            {
                m |= (1 << b) & R;
                if (m >= n)
                    break;
                b++;
            }
            remove(b);
            R -= n;
            rebuild_bit(b);
        }
        else
        {
            int l = M - L, r = M + R;
            r -= n;
            build(l, r);
        }
    }
};

std::vector<std::vector<int>> g;
std::vector<std::pair<int, int>> edges;
std::vector<int> ans, indl, indr;

TrickyDSU build(int l, int r)
{
    return TrickyDSU(std::vector<std::pair<int, int>>(edges.begin() + indl[l], edges.begin() + indr[r]), r - l + 1, l);
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
    edges.clear();
    for (int i = 0; i < M; i++)
    {
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    for (int i = 0; i < N; i++)
    {
        indl[i] = (int)edges.size();
        for (int x : g[i])
        {
            edges.emplace_back(std::min(x, i), std::max(x, i));
        }
        indr[i] = (int)edges.size();
    }

    TrickyDSU big = build(0, N - 1);
    solve(big, 0, N - 1);

    return ans;
}