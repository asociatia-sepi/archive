#include <bits/stdc++.h>

using namespace std;

const int maxn = 1e5;

int p[maxn], d[maxn], deg_sum[maxn], edge_cnt[maxn];
vector<int> g1[maxn], g2[maxn], ans;
int v1[maxn], v2[maxn];

inline int get_deg(int x) { return (int)g1[x].size() - v1[x] + (int)g2[x].size() - v2[x]; }

inline void upd(int x, int l, int r){
    while(v1[x] < (int)g1[x].size() && g1[x][v1[x]] < l) ++v1[x];
    while(v2[x] < (int)g2[x].size() && g2[x][v2[x]] > r) ++v2[x];
    p[x] = -1;
    d[x] = edge_cnt[x] = 0;
    deg_sum[x] = get_deg(x);
}

int get(int x) { return p[x] == -1 ? x : p[x] = get(p[x]); }

int unite(int a, int b){
    a = get(a);
    b = get(b);
    if(a == b){
        edge_cnt[a]++;
    } else{
        if(d[a] < d[b]) swap(a, b);
        p[b] = a;
        if(d[a] == d[b]) d[a]++;
        deg_sum[a] += deg_sum[b];
        edge_cnt[a] += edge_cnt[b] + 1;
    }
    return edge_cnt[a] * 2 == deg_sum[a] ? a : -1;
}

int add(int l, int r, int m1, int m2, int x){
    int res = -1;
    for(int i = v1[x]; i < (int)g1[x].size() && g1[x][i] < m1; i++){
        res = unite(g1[x][i], x);
    }
    for(int i = v2[x]; i < (int)g2[x].size() && g2[x][i] > m2; i++){
        res = unite(g2[x][i], x);
    }
    return deg_sum[x] == 0 ? x : res;
}

void solve(int l, int r){
    if(l == r) {
        ans.push_back(1);
        return;
    }
    int m1 = l, m2 = r, suml = 0, sumr = 0, s = -1;
    upd(m1, l, r);
    upd(m2, l, r);
    while(m1 <= m2 && s == -1){
        int d1 = get_deg(m1);
        int d2 = get_deg(m2);
        if(suml + d1 < sumr + d2){
            suml += d1 + 1;
            m1++;
            s = add(l, r, m1, m2, m1-1);
            if(m1 <= m2) upd(m1, l, r);
        } else{
            sumr += d2 + 1;
            m2--;
            s = add(l, r, m1, m2, m2+1);
            if(m1 <= m2) upd(m2, l, r);
        }
    }
    assert(s != -1);
    int last = l;
    bool split = get(l) == s;
    vector<pair<int, int>> intervals;
    for(int i = l+1; i < m1; i++){
        if((get(i) == s) != split){
            intervals.emplace_back(last, i-1);
            last = i;
            split = !split;
        }
    }
    if(m1 <= m2 && split){
        intervals.emplace_back(last, m1-1);
        last = m1;
        split = false;
    }
    for(int i = m2+1; i <= r; i++){
        if((get(i) == s) != split){
            intervals.emplace_back(last, i-1);
            last = i;
            split = !split;
        }
    }
    if(last == l){
        ans.push_back(r-l+1);
        return;
    }
    intervals.emplace_back(last, r);

    for(auto [x, y] : intervals) solve(x, y);
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y){
    for(int i = 0; i < M; i++){
        g2[X[i]].push_back(Y[i]);
        g1[Y[i]].push_back(X[i]);
    }

    for(int i = 0; i < N; i++){
        sort(g1[i].begin(), g1[i].end());
        sort(g2[i].rbegin(), g2[i].rend());
    }

    solve(0, N-1);
    
    return ans;
}