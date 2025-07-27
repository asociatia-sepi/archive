// indexat de la 0
#include <bits/stdc++.h>

using namespace std;

#define NMAX 1000005
#define LOGMAX 21
#define INF 1000000000

int n,m;
int rmq[2][LOGMAX][NMAX];
int lg[NMAX], LG;
int highest[LOGMAX][NMAX];
int highest_m1[LOGMAX][NMAX];

void calc_rmq(int t)
{
	for(int j = 0; j < n; j ++) {
		rmq[t][0][j] = j;
	}
	for(int i = 1; i <= LG; i ++) {
		for(int j = 0; j + (1 << i) - 1 < n; j ++) {
			int left = rmq[t][i - 1][j];
			int right = rmq[t][i - 1][j + (1 << (i - 1))];
			if(highest[t][left] > highest[t][right]) {
				rmq[t][i][j] = left;
			} else {
				rmq[t][i][j] = right;
			}
		}
	}
}

int rmquery(int l, int r, int t)
{
	int k = lg[r - l + 1];
	int left = rmq[t][k][l];
	int right = rmq[t][k][r - (1 << k) + 1];
	if(highest[t][left] > highest[t][right]) {
		return left;
	}
	return right;
}

int start;

pair<int,int> get_next(int a, int al, int k, bool made_a_jump)
{
	// calculam next_a
	// fara -1
	int next_a = highest[k][a];
	next_a = max(next_a, highest[k][rmquery(start, a, 0)]);
	next_a = max(next_a, highest[k][rmquery(start, a, 1)]);
	// cu -1
	if(made_a_jump) {
		int lol_node = highest[1][rmquery(start, al, 1)];
		lol_node = max(lol_node, highest[1][rmquery(start, al, 0)]);
		
		next_a = max(next_a, lol_node);
		next_a = max(next_a, highest_m1[k][rmquery(start, lol_node, 0)]);
		next_a = max(next_a, highest_m1[k][rmquery(start, lol_node, 1)]);
	}
	// calculam next_al
	int next_al = highest_m1[k][a];
	if(made_a_jump) {
		next_al = max(next_al, highest[k][al]);
		next_al = max(next_al, highest[k][rmquery(start, al, 0)]);
		next_al = max(next_al, highest[k][rmquery(start, al, 1)]);
	}
	next_al = max(next_al, highest_m1[k][rmquery(start, a, 0)]);
	next_al = max(next_al, highest_m1[k][rmquery(start, a, 1)]);

	return {next_a, next_al};
}

int query(int a, int b)
{
	bool made_a_jump = 0;
	start = a;
	
	int ans = INF;
	int sum = 0;
	if(a >= b) { return 0; }
	
	int al = a;  // a lagged, pt ca a facut numarul de salturi - 1
	for(int k = LG + 1; k >= 0; k --) {
		pair<int,int> next = get_next(a, al, k, made_a_jump);
		int next_a = next.first;
		int next_al = next.second;

		if(next_a >= b) {
			ans = min(ans, sum + (1 << k)); // count the jump to solution
			// but don't jump
		} 
		if(next_a < b) {
			made_a_jump = 1;
			sum += (1 << k);
			a = next_a;
			al = next_al;
		}
	}
	return ans;
}

vector<int> solve(vector<int> &v, vector<int> &w, vector<pair<int,int>> &queries)
{
	n = v.size();
	m = queries.size();
	lg[1] = 0;
	for(int i = 2; i < NMAX; i++) {
		lg[i] = lg[i / 2] + 1;
	}
	LG = lg[n];
	
	for(int i = 0; i < n; i++) {
		highest[0][i] = min(n - 1, v[i] + i);
		highest[1][i] = min(n - 1, w[i] + i);
		highest_m1[1][i] = highest[0][i];
		highest_m1[0][i] = i;
	}
	calc_rmq(0);
	for(int i = 0; i < n; i++) {
		highest[1][i] = max(highest[1][i], 
			highest[0][rmquery(i, highest[0][i], 0)]);
	}
	calc_rmq(1);

	for(int k = 2; k <= LG + 1; k++) {
		for(int i = 0; i < n; i++) {
			// full cu full
			int rmq1 = rmquery(i, highest[k - 1][i], 1);
			int rmq0 = rmquery(i, highest[k - 1][i], 0);
			
			highest[k][i] = max(highest[k][i], max(
				highest[k - 1][rmq0],
				highest[k - 1][rmq1]
			));
			
			// m1 cu 2 cu m1
			int rmqm11 = rmquery(i, highest_m1[k - 1][i], 1);
			int rmqm10 = rmquery(i, highest_m1[k - 1][i], 0);
			int best_jump_2 = highest[1][rmqm11];

			// probabil sunt niste calcule redundante, o sa le optimizez mai tarziu
			int next_21 = rmquery(i, best_jump_2, 0);
			int next_22 = rmquery(i, best_jump_2, 1);

			highest[k][i] = max(highest[k][i], max(
				highest_m1[k - 1][next_21],
				highest_m1[k - 1][next_22]
			));

			// full cu m1
			highest_m1[k][i] = max(highest_m1[k][i], max(
				highest_m1[k - 1][rmq0],
				highest_m1[k - 1][rmq1]
			));

			// m1 cu full
			highest_m1[k][i] = max(highest_m1[k][i], max(
				highest[k - 1][rmqm10],
				highest[k - 1][rmqm11]
			));
		}
	}

	vector<int> ans(m);
	for(int i = 0; i < m; i++) {
		int a = queries[i].first;
		int b = queries[i].second;
		ans[i] = query(a, b);
	}
	return ans;
}
