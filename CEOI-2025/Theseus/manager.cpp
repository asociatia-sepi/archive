#include <bits/stdc++.h>
using namespace std;

const int PAINTERS = 3;
int PROGS;
vector<pair<ifstream, ofstream>> fifo;

struct Graph {
    int N, M, destination_node;
    vector<pair<int, int>> edges;
    vector<vector<pair<int, int>>> G;
    int starting_node;
    int max_moves;
};


mt19937 get_rng() {
	seed_seq seq {
        7384562
		// (uint64_t) chrono::duration_cast<chrono::nanoseconds>(
		// 	chrono::high_resolution_clock::now().time_since_epoch()).count(),
		// (uint64_t) __builtin_ia32_rdtsc(),
		// (uint64_t) (uintptr_t) unique_ptr<char>(new char).get()
	};
	return mt19937(seq);
}

auto rng = get_rng();

void send_to(int prog, int x) {
    fifo[prog].second << x << endl;
    // fifo[prog].second.flush();
}

void outcome(float score, string message) {
    for(int i = 0; i < PROGS; i++) {
        send_to(i, -1);
        fifo[i].second.flush();
    }
    cout << score << endl;
    cerr << message << endl;
    exit(0);
}



void send_to(int prog, string x) {
    fifo[prog].second << x << endl;
    fifo[prog].second.flush();
}

void send_graph(int prog, Graph &g) {
    send_to(prog, g.N);
    send_to(prog, g.edges.size());
    send_to(prog, g.destination_node);

    for(auto [x, y] : g.edges) {
        send_to(prog, x);
        send_to(prog, y);
    }
    fifo[prog].second.flush();
}

void read_graph(Graph &g) {
    cin >> g.N >> g.M >> g.destination_node;
    g.edges.resize(g.M);
    g.G.resize(g.N);
    g.destination_node--;
    for(auto &[x, y] : g.edges) {
        cin >> x >> y;
        x--, y--;
    }
    cin >> g.starting_node;
    g.starting_node--;

    cin >> g.max_moves;
}

int read_from(int prog) {
    int x;
    fifo[prog].first >> x;
    return x;
}

void read_paint(int prog, Graph &g) {
    for(int i = 0; i < g.M; i++) {
        int c;
        c = read_from(prog);
        auto [x, y] = g.edges[i];
        if(c != 0 && c != 1) {
            outcome(0.0, "translate:wrong");
        }
        g.G[x].emplace_back(y, c);
        g.G[y].emplace_back(x, c);
    }
}

void send_travel_params(int prog, int node, Graph &g) {
    send_to(prog, node);
    send_to(prog, g.N);
    send_to(prog, g.G[node].size());
    for(auto [x, c] : g.G[node]) {
        send_to(prog, x);
        send_to(prog, c);
    }
    fifo[prog].second.flush();
}

bool check_node(int curr_node, int next_node, Graph &g) {
    for(auto it : g.G[curr_node]) {
        if(it.first == next_node) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    signal(SIGPIPE, SIG_IGN);
    PROGS = (argc - 1) / 2;
    fifo.resize(PROGS);

    for(int i = 0; i < PROGS; i++) {
        fifo[i].first.open(argv[1 + 2 * i]);
        fifo[i].second.open(argv[1 + 2 * i + 1]);
    }

    for(int i = 0; i < PROGS; i++) {
        send_to(i, (i < PAINTERS ? "paint" : "travel"));
        fifo[i].second.flush();
    }
    
    vector<Graph> graphs(PAINTERS);

    for(int i = 0; i < PAINTERS; i++) {
        read_graph(graphs[i]);
        send_graph(i, graphs[i]);
        read_paint(i, graphs[i]);
    }
    
    uniform_int_distribution<int> gen(0, 1e9);
    
    int curr_node = graphs[0].starting_node;
    int max_moves = graphs[0].max_moves;
    while(max_moves--) {
        int asking = gen(rng) % (PROGS - PAINTERS) + PAINTERS;

        for(int i = PAINTERS; i < PROGS; i++) {
            int curr_graph = 0;
            int asking_node = curr_node;
            if(i != asking) {
                curr_graph = 1 + gen(rng) % (PAINTERS - 1);
                asking_node = gen(rng) % (graphs[curr_graph].N);
                while(asking_node == graphs[curr_graph].destination_node) {
                    asking_node = gen(rng) % (graphs[curr_graph].N);
                }
            }
            
            send_travel_params(i, asking_node, graphs[curr_graph]);
            int x = read_from(i);
            if(!check_node(asking_node, x, graphs[curr_graph])) {
                outcome(0.0, "translate:wrong");
            }
            if(i == asking) {
                curr_node = x;
                // cerr << curr_node << "\n";
            }
        }

        if(curr_node == graphs[0].destination_node) {
            outcome(1.0, "translate:success");
        }
    }
    outcome(0.0, "translate:wrong");
    return 0;
}