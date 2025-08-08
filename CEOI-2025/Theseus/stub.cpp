#include <bits/stdc++.h>

using namespace std;

int travel(int n, int node, std::vector<std::pair<int,int>> directions);
std::vector<int> paint(int n, std::vector<std::pair<int,int>> edges, int root);

int main(int argc, char *argv[]) {
    ofstream out(argv[2]);
    ifstream in(argv[1]);
    
    string type;
    in >> type;
    int N, M, target;
    if(type == "paint") {
        vector<pair<int, int>> edges;
        in >> N >> M >> target;
        if(N < 0 || M < 0 || target < 0) {
            return 0;
        }
        target++;
        edges.resize(M);
        for(auto &[x, y] : edges) {
            in >> x >> y;
            if(x < 0 || y < 0) {
                return 0;
            }
            x++, y++;
        }

        auto col = paint(N, edges, target);
        col.resize(M, 2);
        for(int i = 0; i < M; i++) {
            out << col[i] << "\n";
        }
        out.flush();
    }

    while(true) {
        int node = -1;
        in >> node;
        if(node < 0) {
            return 0;
        }
        int N;
        in >> N;
        if(N < 0) {
            return 0;
        }
        node++;
        int size = 0;
        in >> size;
        if(size < 0) {
            return 0;
        }
        vector<pair<int, int>> directions(size);
        for(int i = 0; i < size; i++) {
            in >> directions[i].first >> directions[i].second;
            if(directions[i].first < 0 || directions[i].second < 0) {
                return 0;
            }
            directions[i].first++;
        }

        int ret = travel(N, node, directions);
        out << ret - 1 << "\n";
        out.flush();
    }
    return 0;
}