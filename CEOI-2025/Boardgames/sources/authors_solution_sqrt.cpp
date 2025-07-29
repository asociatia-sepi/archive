
#include <bits/stdc++.h>

#ifdef EVAL 
#include "boardgames.h"
#endif

using namespace std;
const int B = 512;


struct DSU {
  vector<int> link, timer;
  int t = 0;

  int Find(int x) {
    if (timer[x] != t) {
      timer[x] = t;
      link[x] = -1;
      return x;
    } else if (link[x] == -1) return x;
    return link[x] = Find(link[x]);
  }
  bool Unite(int x, int y) {
    x = Find(x); y = Find(y);
    if (x == y) return false;
    link[x] = y;
    return true;
  }
  void Init(int n) {
    link.assign(n, -1);
    timer.assign(n, -1);
    t = 0;
  }
  void Clear() { ++t; }
};

vector<vector<int>> graph;
vector<int> ans;

DSU dsu1, dsu2;
int rebuilds = 0;

void divide(int b, int e, int rb, int re) {
  if (rb < b || re > e) {
    rb = re = -1;
    dsu1.Clear();
  }

  if (rb == -1 && e - b > 3 * B) {
    rebuilds += 1;
    rb = ((b / B) + 1) * B;
    re = ((e / B) - 1) * B;
    assert(rb < re);
    for (int i = rb; i < re; ++i) {
      for (auto j : graph[i]) {
        if (j < rb || j >= re) continue;
        dsu1.Unite(i, j);
      }
    }
  }

  dsu2.Clear();
  for (int i = b; i < e; ++i) {
    if (i == rb) i = re;
    for (auto j : graph[i]) {
      if (j < b || j >= e) continue;
      dsu2.Unite(dsu1.Find(i), dsu1.Find(j));
    }
  }

  for (int i = b, j = e - 1; i < j; ++i, --j) {
    if (dsu2.Find(dsu1.Find(i)) != dsu2.Find(dsu1.Find(i + 1))) {
      divide(i + 1, e, rb, re);
      divide(b, i + 1, -1, -1);
      return;
    }
    if (dsu2.Find(dsu1.Find(j - 1)) != dsu2.Find(dsu1.Find(j))) {
      divide(b, j, rb, re);
      divide(j, e, -1, -1);
      return;
    }
  }

  for (int i = b; i + 1 < e; ++i) {
    assert(dsu2.Find(dsu1.Find(i)) == dsu2.Find(dsu1.Find(i + 1)));
  }
  
  ans[b] = 1;
}

vector<int> partition_players(int n, int m, vector<int> X, vector<int> Y) {
  // Make graph such that each node has at most 3 incident edges.
  vector<int> deg(n, 0);
  
  vector<int> dX(m), dY(m);
  for (int i = 0; i < m; ++i) {
    dX[i] = (deg[X[i]]++) / 2;
    dY[i] = (deg[Y[i]]++) / 2;
  }

  vector<int> remap(n + 1, 0);
  for (int i = 0; i < n; ++i) 
    remap[i + 1] = remap[i] + deg[i] / 2 + 1;

  int nodes = remap.back();
  graph.assign(nodes, {});
  for (int i = 0; i < n; ++i) {
    for (int j = remap[i] + 1; j < remap[i + 1]; ++j) {
      graph[j - 1].push_back(j);
      graph[j].push_back(j - 1);
    }
  }
  for (int i = 0; i < m; ++i) {
    int x = remap[X[i]] + dX[i];
    int y = remap[Y[i]] + dY[i];
    graph[x].push_back(y);
    graph[y].push_back(x);
  }

  ans.assign(nodes, 0);
  dsu1.Init(nodes);
  dsu2.Init(nodes);
  divide(0, nodes, -1, -1);
  vector<int> K;
  for (int i = 0; i < n; ++i) {
    if (ans[remap[i]] == 0) ++K.back();
    else K.push_back(1);
  }
  return K;
}

#ifndef EVAL 
int main() {
  int n, m; cin >> n >> m;
  vector<int> X(m), Y(m);
  for (int i = 0; i < m; ++i) {
    cin >> X[i] >> Y[i];
    --X[i]; --Y[i];
  }
  auto K = partition_players(n, m, X, Y);
  for (int i = 0; i < (int)K.size(); ++i)
    for (int j = 0; j < K[i]; ++j)
      cout << i + 1 << " ";
  cout << endl;
  cerr << rebuilds << " rebuilds" << endl;

  return 0;
}
#endif