#include <iostream>
#include <vector>
#include <utility>

std::vector<int> solve(std::vector<int> &v, std::vector<int> &w,
	std::vector<std::pair<int,int>> &queries);

/**
 * Read from standard input the testcase in the following format:
 *
 * n
 * v[0] v[1] ... v[n - 1]
 * w[0] w[1] ... w[n - 1]
 * m
 * A[0] B[0]
 * A[1] B[1]
 * ...
 * A[m - 1] B[m - 1]
 */

int main() {
    int n;

    std::cin >> n;
    std::vector<int> v(n), w(n);

    for (int i = 0; i < n; i++)
        std::cin >> v[i];
    for (int i = 0; i < n; i++)
        std::cin >> w[i];

    int m;
    std::cin >> m;
    std::vector<std::pair<int, int>> queries(m);
    for (int i = 0; i < m; i++)
        std::cin >> queries[i].first >> queries[i].second;

    std::vector<int> res = solve(v, w, queries);

    for (int i = 0; i < res.size(); i++)
        std::cout << res[i] << "\n";

    return 0;
}

