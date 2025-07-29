#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> g1, g2;
vector<int> v1, v2, vis, use, bad, group;
int last_vis = 1, group_id = 1;

inline int get_side(int n){
    return sqrt(n);
}

inline void upd(int x, int l, int r){
    while(v1[x] < (int)g1[x].size() && g1[x][v1[x]] < l) ++v1[x];
    while(v2[x] < (int)g2[x].size() && g2[x][v2[x]] >= r) --v2[x];
}

bool dfs(int x, int l, int r, int c){
    vis[x] = last_vis;
    group[x] = c;
    if(v1[x] < (int)g1[x].size() && g1[x][v1[x]] < l){
        bad[x] = last_vis;
        return false;
    }
    if(v2[x] < (int)g2[x].size() && r <= g2[x][v2[x]]){
        bad[x] = last_vis;
        return false;
    }
    for(int i = v1[x]; i < (int)g1[x].size(); i++){
        if(vis[g1[x][i]] != last_vis){
            if(!dfs(g1[x][i], l, r, c)){
                bad[x] = last_vis;
                return false;
            }
        }
    }
    for(int i = v2[x]; i < (int)g2[x].size(); i++){
        if(vis[g2[x][i]] != last_vis){
            if(!dfs(g2[x][i], l, r, c)){
                bad[x] = last_vis;
                return false;
            }
        }
    }
}

void solve(int l, int r){
    int m = get_side(r-l);
    for(int i = l; i < l+m; i++) upd(i, l, r);
    for(int i = r-m; i < r; i++) upd(i, l, r);
    int split = -1;
    for(int i = l; i < l+m && split == -1; i++){
        if(dfs(i, l, l + m, group_id++)){
            split = group_id - 1;
        }
    }
    if(split != -1){
        
    }
    for(int i = r-m; i < r && split == -1; i++){
        if(dfs(i, r-m, r, group_id++)){
            split = group_id - 1;
        }
    }
}

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y){
    g1.resize(N);
    g2.resize(N);
    v1.resize(N);
    v2.resize(N);
    vis.resize(N);
    use.resize(N);
    bad.resize(N);
    group.resize(N);
    for(int i = 0; i < M; i++){
        g2[X[i]].push_back(Y[i]);
        g1[Y[i]].push_back(X[i]);
    }

    for(int i = 0; i < N; i++){
        sort(g2[i].rbegin(), g2[i].rend());
        sort(g1[i].begin(), g1[i].end());
    }
    
    return {};
}