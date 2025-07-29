#include <bits/stdc++.h>
#include "lawn.h"

using namespace std;

static int _in_loc; char _in_buff[4096];

static void read_init()
{
	_in_loc = 4095;
}

static char read_ch(FILE* fin)
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

static int read_u32(FILE* fin)
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

static int _out_loc; char _out_buff[50000];
 
static void write_init()
{
    _out_loc = 0;
}
 
static void write_ch(FILE* fout, char ch)
{
    if (_out_loc == 50000) {
        fwrite(_out_buff, 1, 50000, fout);
        _out_loc = 0;
        _out_buff[_out_loc++] = ch;
    } else {
        _out_buff[_out_loc++] = ch;
    }
}
 
static void write_u64(FILE* fout, uint64_t vu64)
{
    if (vu64 <= 9) {
        write_ch(fout, vu64 + '0');
    } else {
        write_u64(fout, vu64 / 10);
        write_ch(fout, vu64 % 10 + '0');
    }
}
 
static void write_appendix(FILE* fout)
{
    fwrite(_out_buff, 1, _out_loc, fout);
}

int main() {

    read_init();
    write_init();

    FILE* fin = fopen("lawn.in", "r");
    FILE *fout = fopen("lawn.out", "w");

    assert(fin != NULL);
    assert(fout != NULL);

	int n, c, b;
	n = read_u32(fin);
	c = read_u32(fin);
	b = read_u32(fin);

    vector<int> a(n), v(n);

	for (int i = 0; i < n; ++i) {
		a[i] = read_u32(fin);
	}
	for (int i = 0; i < n; ++i) {
		v[i] = read_u32(fin);
	}

    long long ans = mow(n, c, b, a, v);
    write_u64(fout, ans);
    write_ch(fout, '\n');
    write_appendix(fout);

    fclose(fin);
    fclose(fout);

	return 0;
}
