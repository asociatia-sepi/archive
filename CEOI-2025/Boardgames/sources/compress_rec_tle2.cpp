#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <map>

struct DSU{
    std::vector<int> p, h, sz;
    void init(int n){
        p.assign(n, -1);
        h.assign(n, 0);
        sz.assign(n, 1);
    }
    void reset(int l, int r){
        std::fill(p.begin() + l, p.begin() + r, -1);
        std::fill(h.begin() + l, h.begin() + r, 0);
        std::fill(sz.begin() + l, sz.begin() + r, 1);
    }
    int get(int x) { return p[x] == -1 ? x : p[x] = get(p[x]); }
    int getsz(int x) { return sz[get(x)]; }
    bool unite(int x, int y){
        x = get(x), y = get(y);
        if(x == y) return false;
        if(h[x] < h[y]) std::swap(x, y);
        p[y] = x;
        sz[x] += sz[y];
        if(h[x] == h[y]) h[x]++;
        return true;
    }
};

const int B = 300;

std::pair<std::vector<std::vector<int>>, std::vector<int>> compress(const std::vector<std::vector<int>>& g, const std::vector<int> &gsz){
    int N = (int)g.size();
    DSU dsu;
    dsu.init(N);
    int i = 0;
    int ind = 0;
    std::vector<std::vector<int>> g2;
    std::vector<int> p(N, -1), gsz2 = {0};
    while(i < N){
        int cnt = 0, i2 = i+1, j = i+1;
        while(j < N && cnt < B){
            for(int x : g[j]){
                if(i <= x && x < j){
                    dsu.unite(j, x);
                }
            }
            cnt += std::max((int)g[j].size(), 1);
            if(dsu.getsz(i) == j-i+1){
                i2 = j+1;
                cnt = 0;   
            }
            ++j;
        }
        for(int k = i; k < i2; k++) p[k] = ind;
        gsz2.push_back(gsz[i2] - gsz[i]);
        ind++;
        dsu.reset(i2, j);
        i = i2;
    }

    g2.assign(ind, std::vector<int>());
    for(int j = 0; j < N; j++){
        for(int x : g[j]){
            if(x < j){
                g2[p[x]].push_back(p[j]);
                g2[p[j]].push_back(p[x]);
            }
        }
    }

    for(int j = 0; j < ind; j++){
        std::sort(g2[j].begin(), g2[j].end());
        g2[j].resize(std::unique(g2[j].begin(), g2[j].end()) - g2[j].begin());
    }

    for(int j = 1; j <= ind; j++){
        gsz2[j] += gsz2[j-1];
    }
    return {g2, gsz2};
}

std::pair<std::vector<std::vector<int>>, std::vector<int>> cut_graph(const std::vector<std::vector<int>>& g, const std::vector<int> &gsz, int L, int R){
    std::vector<std::vector<int>> g2(R-L);
    std::vector<int> gsz2(R-L+1);

    for(int i = L; i < R; i++){
        for(int x : g[i]){
            if(L <= x && x < R){
                g2[i-L].push_back(x - L);
            }
        }
    }

    for(int i = L; i <= R; i++){
        gsz2[i-L] = gsz[i] - gsz[L];
    }

    return {g2, gsz2};
}

std::vector<int> ans;

void dfs(const std::vector<std::vector<int>> &g, std::vector<int> &group, int x, int c){
    group[x] = c;
    for(int y : g[x]){
        if(group[y] == -1){
            dfs(g, group, y, c);
        }
    }
}

void partition(const std::vector<std::vector<int>> &g_, const std::vector<int> &gsz_){
    if(g_.size() < B){
        ans.resize((int)ans.size() + (int)g_.size(), 1);
        return;
    }
    auto [g, gsz] = compress(g_, gsz_);
    std::vector<int> group(g.size(), -1);
    for(int i = 0; i < (int)g.size(); i++){
        if(group[i] == -1){
            dfs(g, group, i, i);
        }
    }
    int last = 0;
    for(int i = 1; i < (int)g.size(); i++){
        if(group[i] != group[last]){
            auto cg = cut_graph(g, gsz, last, i);
            partition(cg.first, cg.second);
            last = i;
        }
    }
    if(last == 0) {
        ans.push_back(gsz.back());
    } else {
        auto cg = cut_graph(g, gsz, last, (int)g.size());
        partition(cg.first, cg.second);
    }
    return;
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y){
    std::vector<std::vector<int>> g(N);
    for(int i = 0; i < M; i++){
        g[X[i]].push_back(Y[i]);
        g[Y[i]].push_back(X[i]);
    }
    std::vector<int> gsz(N+1);
    std::iota(gsz.begin(), gsz.end(), 0);
    partition(g, gsz);
    return ans;
}