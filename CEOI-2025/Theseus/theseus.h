#pragma once
#include <vector>

/*
Receives n(the number of nodes), a list of the edges in the graph, and the root that must be reached.
Returns a list of colors, where the ith colors is for the ith edge.
*/
std::vector<int> paint(int n, std::vector<std::pair<int,int>> edges, int root);

/*
Receives n(the number of nodes), a list of possible directions (where each direction is represented by the destination node and the colors of the edge) 
Returns the index of the taken direction from the list of directions
*/

int travel(int n, int node, std::vector<std::pair<int,int>> directions);