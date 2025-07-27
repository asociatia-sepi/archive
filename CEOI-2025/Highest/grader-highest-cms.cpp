#include <bits/stdc++.h>

using namespace std;

static int _in_loc; 
static char _in_buff[4096];

static void read_init()
{
	_in_loc = 4095;
}

static char read_ch(FILE *fin)
{
	++_in_loc;
	if (_in_loc == 4096) {
		_in_loc = 0;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
		fread(_in_buff, 1, 4096, fin);
#pragma GCC diagnostic pop
	}
	return _in_buff[_in_loc];
}

static int read_u32(FILE *fin)
{
	int u32 = 0; char c;
	while (!isdigit(c = read_ch(fin)) && c != '-');
	int sgn = 1;
	if (c == '-') {
		sgn = -1;
	} else {
		u32 = c - '0';
	}
	while (isdigit(c = read_ch(fin))) {
		u32 = u32 * 10 + c - '0';
	}
	return u32 * sgn;
}

static int _out_loc; 
static char _out_buff[50000];
 
static void write_init()
{
    _out_loc = 0;
}
 
static void write_ch(FILE* fout, char ch)
{
    if (_out_loc == 50000) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
        fwrite(_out_buff, 1, 50000, fout);
#pragma GCC diagnostic pop

        _out_loc = 0;
        _out_buff[_out_loc++] = ch;
    } else {
        _out_buff[_out_loc++] = ch;
    }
}
 
static void write_u32(FILE* fout, unsigned int vu32)
{
    if (vu32 <= 9) {
        write_ch(fout, vu32 + '0');
    } else {
        write_u32(fout, vu32 / 10);
        write_ch(fout, vu32 % 10 + '0');
    }
}
 
static void write_appendix(FILE* fout)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    fwrite(_out_buff, 1, _out_loc, fout);
#pragma GCC diagnostic pop
}

vector<int> solve(vector<int> &v, vector<int> &w, vector<pair<int,int>> &queries);

int main() {
    
    FILE* fin = fopen("highest.in", "r");
    FILE* fout = fopen("highest.out", "w");

    read_init();
    write_init();

	int N, M;
	N = read_u32(fin);

	vector<int> v(N), w(N);

	for (int i = 0; i < N; ++i) {
		v[i] = read_u32(fin);
	}
	for (int i = 0; i < N; ++i) {
		w[i] = read_u32(fin);
	}

	M = read_u32(fin);
	vector<pair<int, int>> queries(M);
	for (int i = 0; i < M; ++i) {
		queries[i].first = read_u32(fin);
		queries[i].second = read_u32(fin);
	}

	vector<int> ans = solve(v, w, queries);
	for (int i = 0; i < ans.size(); ++i) {
		write_u32(fout, ans[i]);
		write_ch(fout, '\n');
	}
	write_appendix(fout);

    fclose(fin);
    fclose(fout);

	return 0;
}
