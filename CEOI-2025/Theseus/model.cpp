#include <random>
#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
#include <cassert>

using ll = long long;

struct Edge{
  int to;
  int id;
};

std::vector<int> compute_level(int n, int root, std::vector<std::vector<Edge>> &g) {
  std::vector<int> level(1 + n);
  std::queue<int> q;
  level[root] = 1;
  q.push(root);

  while(0 < q.size()) {
    int node = q.front();
    q.pop();

    for(int h = 0; h < g[node].size(); h++) {
      int to = g[node][h].to;

      if(level[to] == 0) {
        level[to] = level[node] + 1;
        q.push(to);
      }
    }
  }
  return level;
}

int const nmax = 100000;
int level_global[1 + nmax], historic_size[1 + nmax];

std::vector<int> paint(int n, std::vector<std::pair<int,int>> edges, int root) {
  std::vector<int> colors(edges.size());
  std::vector<std::vector<Edge>> g(n + 1);
  for(int h = 0; h < edges.size(); h++) {
    int x, y;
    x = edges[h].first;
    y = edges[h].second;
    g[x].push_back({y, h});
    g[y].push_back({x, h});
  }
  std::vector<int> level = compute_level(n, root, g);

  for(int i = 1;i <= n; i++)
    level_global[i] = level[i];

  std::vector<int> indeces(edges.size());
  std::iota(indeces.begin(), indeces.end(), 0);

  //The level of an edge is determined by the deepest level adjacent to it
  auto edgeLevel = [&](std::pair<int,int> &edge) {
    return std::max(level[edge.first], level[edge.second]);
  };

  //All edges must match to a different edge priority
  //There must exist an ordering of the edges that is consistent with all edgePriority calculations.
  //^edgePriority(x, y) does not need to be edgePriority(y, x), but it makes the above trivial
  //also edgePriority must be sync-ed between A and B 
  auto edgePriority = [&](std::pair<int,int> &edge) {
    if(edge.first < edge.second)
      return 1LL * n * edge.first + edge.second;
    else
      return 1LL * n * edge.second + edge.first;
  };

  //Sort edges in decreasing order of height and then decreasing order of priority
  std::sort(indeces.begin(), indeces.end(), [&](int x, int y) {
      if(edgeLevel(edges[x]) != edgeLevel(edges[y]))
        return edgeLevel(edges[x]) < edgeLevel(edges[y]);
      else
        return edgePriority(edges[x]) < edgePriority(edges[y]);
  }); 
  std::reverse(indeces.begin(), indeces.end());

  std::vector<int> sub_size(n + 1);
  std::fill(sub_size.begin() + 1, sub_size.end(), 1);

  for(auto index: indeces) {
    std::pair<int,int> edge = edges[index];
    int x = edge.first;
    int y = edge.second;
    if(y < x)
      std::swap(x, y);
    bool towardsX = (level[x] < level[y]) || (level[x] == level[y] && sub_size[x] > sub_size[y]);
    if(towardsX) {
      if(sub_size[y] != 0)
        historic_size[y] = sub_size[y];

      colors[index] = 0;
      sub_size[x] += sub_size[y];
      sub_size[y] = 0;
    } else {
      if(sub_size[x] != 0)
        historic_size[x] = sub_size[x];

      colors[index] = 1;
      sub_size[y] += sub_size[x];
      sub_size[x] = 0;
    }
  }
  return colors;
}

int travel(int n, int node, std::vector<std::pair<int,int>> directions) {
  int chosen_direction = -1;
  ll current_priority = -1;

  //All edges must match to a different edge priority
  auto edgePriority = [&](std::pair<int,int> edge) {
    if(edge.first < edge.second)
      return 1LL * n * edge.first + edge.second;
    else
      return 1LL * n * edge.second + edge.first;
  };

  for(int h = 0; h < directions.size(); h++) {
    int to = directions[h].first;
    int color = directions[h].second;
    bool points_outward = (node < to) == color;

    if(points_outward) {
      ll potential_priority = edgePriority({node, to});
      bool has_bigger_priority = (current_priority < potential_priority);

      if (has_bigger_priority) { 
        chosen_direction = h;
        current_priority = potential_priority;
      }
    }
  }

  assert(0 <= chosen_direction); //There should be some way
  if(chosen_direction == -1)
    return 0;

  int to = directions[chosen_direction].first;
  return to;
}
