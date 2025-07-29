#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> pii;

std::vector<int> partition_players(int N, int M, std::vector<int> X, std::vector<int> Y)
{
    set<pii> edge;
    vector<int> ans;
    for (int i = 0; i < M; ++i)
    {
        edge.insert({X[i], Y[i]});
    }
    int l = 0;
    for (int i = 1; i < N; ++i)
    {
        if (!edge.count({i - 1, i}))
        {
            ans.push_back(i - l), l = i;
        }
    }
    ans.push_back(N - l);
    return ans;
}