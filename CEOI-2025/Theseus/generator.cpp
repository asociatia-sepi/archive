#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <algorithm>
#include <ctime>
#include <queue>
#include <cassert>

using namespace std;

using Edge = std::pair<int, int>;

/*
 *Returns the levels(distance from root) of the all the nodes in the graph
 *
*/
std::vector<int> get_levels(int n, int root, std::vector<std::vector<int>> &g) {
  std::vector<int> level(n + 1);
  std::queue<int> q;
  level[root] = 1;
  q.push(root);

  while(0 < q.size()) {
    int node = q.front();
    q.pop();

    for(int h = 0; h < g[node].size(); h++) {
      int to = g[node][h];
      if(level[to] == 0) {
        level[to] = level[node] + 1;
        q.push(to);
      }
    }
  }

  return level;
}

/*
 *Checks whether the input represents a valid input
 *A valid input is described as a connected graph
*/
bool verify_input(int n, std::vector<Edge> edges, int root) {
  std::vector<std::vector<int>> g(1 + n);
  for(int h = 0; h < edges.size(); h++) {
    Edge edge = edges[h];
    int x = edge.first;
    int y = edge.second;

    g[x].push_back(y);
    g[y].push_back(x);
  }
  std::vector<int> level = get_levels(n, root, g);
  bool isValid = true;
  
  for(int i = 1;i <= n; i++) 
    isValid &= (0 < level[i]);
  return isValid;
}

struct Graph{
  int n;
  int destination;
  int start;
  int optimal_length;
  std::vector<std::pair<int,int>> edges;
};

Graph generate_tree(int n) {
  Graph result;
  result.n = n;

  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> dist_node(1, n);
  vector<Edge> edges;
  set<pair<int, int>> edge_set;
  
  result.destination = dist_node(rng);
  result.start = dist_node(rng);

  // Tree: n - 1 edges, ensure connectivity
  vector<int> nodes(n);
  iota(nodes.begin(), nodes.end(), 1);
  shuffle(nodes.begin(), nodes.end(), rng);

  for (int i = 1; i < n; ++i) {
    int u = nodes[i];
    int v = nodes[rng() % i];  // Connect to a random earlier node
    edges.emplace_back(u, v);
  }

  result.edges = edges;
  return result;
}

Graph generate_clique(int n) {
  Graph result;
  result.n = n;

  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> dist_node(1, n);
  vector<Edge> edges;
  set<pair<int, int>> edge_set;
  
  result.destination = dist_node(rng);
  result.start = dist_node(rng);

  // Complete graph: all pairs
  for (int u = 1; u <= n; ++u) {
    for (int v = u + 1; v <= n; ++v) {
      edges.emplace_back(u, v);
    }
  }

  result.edges = edges;
  return result;
}

Graph generate_probability_graph(int n) {
  int const edge_probability = 7;

  Graph result;
  result.n = n;

  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> dist_node(1, n);
  vector<Edge> edges;
  set<pair<int, int>> edge_set;
  
  result.destination = dist_node(rng);
  result.start = dist_node(rng);
  // Random edges: choose m <= n*(n-1)/2 unique edges

  //Generate a random tree to ensure connectivity
  vector<int> nodes(n);
  iota(nodes.begin(), nodes.end(), 1);
  shuffle(nodes.begin(), nodes.end(), rng);

  for (int i = 1; i < n; ++i) {
    int u = nodes[i];
    int v = nodes[rng() % i];  // Connect to a random earlier node
    edges.emplace_back(u, v);
    edge_set.insert(minmax(u, v));
  }
  //Augment with additional edges
  int m = n * (n - 1) / 2 / edge_probability;
  while ((int)edges.size() < m) {
    int u = dist_node(rng);
    int v = dist_node(rng);
    if (u == v) continue;
    auto e = minmax(u, v);
    if (edge_set.count(e)) continue;
    edge_set.insert(e);
    edges.emplace_back(e);
  }
  result.edges = edges;
  return result;
}

Graph generate_layered(int n, int cross_layer_probability, int intra_layer_probability) { 
  Graph result;
  result.n = n;

  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> dist_node(1, n);
  vector<Edge> edges;
  set<pair<int, int>> edge_set;
  
  result.destination = dist_node(rng);
  result.start = dist_node(rng);

  vector<vector<int>> layers;
  vector<int> nodes(n);
  iota(nodes.begin(), nodes.end(), 1);

  nodes.erase(std::find(nodes.begin(), nodes.end(), result.destination));
  shuffle(nodes.begin(), nodes.end(), rng);

  layers.push_back({result.destination});

  // Create random-sized layers (up to 5 per layer)
  for (int i = 0; i < n;) {
    int remaining = n - i;
    uniform_int_distribution<int> layer_size_dist(1, min(5, remaining));
    int layer_size = layer_size_dist(rng);

    vector<int> layer(nodes.begin() + i, nodes.begin() + i + layer_size);
    layers.push_back(layer);
    i += layer_size;
  }

  // Enforce required edges: each node in layer i (>0) connects to at least one in layer i-1
  for (size_t i = 1; i < layers.size(); ++i) {
    const auto& prev = layers[i - 1];
    const auto& curr = layers[i];

    for (int u : curr) {
      // Required: connect to at least one from previous layer
      uniform_int_distribution<int> prev_dist(0, prev.size() - 1);
      int v = prev[prev_dist(rng)];
      auto e = minmax(u, v);
      if (!edge_set.count(e)) {
        edges.emplace_back(e);
        edge_set.insert(e);
      }

      // Optional: more edges to other nodes in the previous layer
      if(cross_layer_probability == 0) //0 means skip entirely cross edges
        continue;

      for (int other : prev) {
        if (other == v || rng() % cross_layer_probability == 0) continue;
        auto extra = minmax(u, other);
        if (!edge_set.count(extra)) {
          edges.emplace_back(extra);
          edge_set.insert(extra);
        }
      }
    }
  }
  
  if(0 < intra_layer_probability) {
    for (const auto& layer : layers) {
      for (size_t i = 0; i < layer.size(); ++i) {
        for (size_t j = i + 1; j < layer.size(); ++j) {
          if (rng() % intra_layer_probability) {
            auto e = minmax(layer[i], layer[j]);
            if (!edge_set.count(e)) {
              edges.emplace_back(e);
              edge_set.insert(e);
            }
          }
        }
      }
    }
  }

  result.edges = edges;
  return result;
}

Graph generate_ladder(int n) {
  Graph result;
  result.n = n;

  random_device rd;
  mt19937 rng(rd());
  uniform_int_distribution<int> dist_node(1, n);
  vector<Edge> edges;
  set<pair<int, int>> edge_set;
  
  result.start = dist_node(rng);
  
  assert(n % 2 == 1 && "ladder testcase must have odd number of nodes"); //This testcase can only have n even

  vector<int> nodes(n);
  iota(nodes.begin(), nodes.end(), 1);
  shuffle(nodes.begin(), nodes.end(), rng);

  int half = n / 2;

  //intra-level edges
  for(int i = 1; i <= half; i++)
    edges.emplace_back(nodes[i], nodes[i + half]);

  //Edges that make up the ladder chains
  for(int i = 1; i < half; i++)
    edges.emplace_back(nodes[i], nodes[i + 1]);

  for(int i = 1;i < half; i++)
    edges.emplace_back(nodes[i + half], nodes[i + half + 1]);

  //Connect ends to target
  edges.emplace_back(nodes[0], nodes[1]);
  edges.emplace_back(nodes[0], nodes[1 + half]);
  result.edges = edges;

  result.destination = nodes[0]; //set the root to fixed element at the end
  return result;
}

std::vector<std::vector<int>> build_adjacency_lists(Graph graph) {
  std::vector<std::vector<int>> g;
  g.resize(1 + graph.n);

  for(int i = 0; i < graph.edges.size(); i++) {
    int x = graph.edges[i].first;
    int y = graph.edges[i].second;
    g[x].push_back(y);
    g[y].push_back(x);
  }
  return g;
}

Graph generate_graph(int n, int test_type) {
  Graph graph;
  if(test_type == 1) 
    graph = generate_tree(n); 
  else if(test_type == 2) 
    graph = generate_clique(n);
  else if(test_type == 3) 
    graph = generate_probability_graph(n);
  else if(test_type == 4) 
    graph = generate_layered(n, 2, 0);
  else if(test_type == 5)
    graph = generate_layered(n, 2, 3);
  else if(test_type == 6)
    graph = generate_ladder(n);

  
  std::vector<std::vector<int>> g = build_adjacency_lists(graph);
  std::vector<int> levels = get_levels(graph.n, graph.destination, g);
   
  graph.optimal_length = levels[graph.start];
  return graph;
}

void print_graph(Graph &graph) {
  std::cout << graph.n << " " << graph.edges.size() << " " << graph.destination << '\n';
  for(int i = 0; i < graph.edges.size(); i++) {
    std::cout << graph.edges[i].first << " " << graph.edges[i].second << '\n';
  }
  std::cout << graph.start << " " << graph.optimal_length - 1 << '\n';
}

int main() {
  int n, test_type, mistakes;
  cin >> n >> test_type >> mistakes;
  /*
  1) tree
  2) clique
  3) 1 / 7 probability
  4) bipartite
  5) general
  6) ladder
  */
  if (n <= 0 || (test_type < 1 || test_type > 6)) {
    cerr << "Invalid input\n";
    return 1;
  }
  for(int tries = 1; tries <= 3; tries++) {
    Graph graph = generate_graph(n, test_type);
    assert(verify_input(n, graph.edges, graph.destination));
    graph.optimal_length += mistakes;

    print_graph(graph);
  }

  return 0;
}
