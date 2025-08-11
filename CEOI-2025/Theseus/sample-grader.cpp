#include <iostream>
#include <vector>

/*
Receives n(the number of vertices), a list of the edges in the graph, and the target that must be reached.
Returns a list of colors, where the ith colors is for the ith edge (0-indexed).
*/
std::vector<int> paint(int n, std::vector<std::pair<int,int>> edges, int t);

/*
Receives n(the number of vertices), a node u and the list of its neighbours (where each neihgbour is represented by the adjacent vertex and the color of the corresponding edge) 
Returns the vertex chosen to move to
*/
int travel(int n, int u, std::vector<std::pair<int,int>> neighbours);

int main() {
    int n, m, s, t;
    std::cin >> n >> m >> s >> t;

    std::vector<std::vector<std::pair<int,int>>> graph(n + 1);
    std::vector<std::pair<int, int>> edges(m);

    for(auto &[x, y] : edges) {
        std::cin >> x >> y;
    }

    std::vector<int> labels = paint(n, edges, t);

    for(int i = 0; i < m; i++) {
        auto [x, y] = edges[i];
        graph[x].emplace_back(y, labels[i]);
        graph[y].emplace_back(x, labels[i]);
    }

    int curr_node = s;
    int moves = 0;

    while(curr_node != t) {
        moves++;
        int next_node = travel(n, curr_node, graph[curr_node]);
        std::cout << "Moving from " << curr_node << " to " << next_node << "\n";
        curr_node = next_node;
    }

    std::cout << "Path length: " << moves << "\n";
    return 0;
}
