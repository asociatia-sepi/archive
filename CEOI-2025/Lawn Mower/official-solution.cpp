#include <algorithm>
#include <iostream>
#include <limits>
#include <ranges>
#include <vector>
#include <cassert>
 
#include "lawn.h"
 
using namespace std;

const int kMaxN = 2e5;
const int kMaxA = 1e9;
const int kMaxB = 1e9;
const int kMaxC = 1e9;
const int kMaxV = 1e9;
 
int divceil(int A, int B) { return A / B + (A % B != 0); }
 
class SegmentTree {
public:
  SegmentTree(int size, int64_t def) : size(size), data(2 * size, Data{def}) {}
 
  void set(int pos, int64_t value) {
    visit(1, 0, size, [&](int l, int r, Data &data) {
      if (r <= pos || pos < l)
        return false;
      if (r - l == 1) {
        data.add = data.min = value;
        return false;
      }
      value -= data.add;
      return true;
    });
  }
 
  void update(int from, int to, int64_t add) {
    if (from == to)
      return;
    visit(1, 0, size, [&](int l, int r, Data &data) {
      if (r <= from || to <= l)
        return false;
      if (from <= l && r <= to) {
        data.add += add;
        data.min += add;
        return false;
      }
      return r - l > 1;
    });
  }
 
  int64_t min() const { return data[1].min; }
 
  void visit(int node, int begin, int end, auto &&cb) {
    if (!cb(begin, end, data[node]))
      return;
 
    int mid = (begin + end) / 2;
    int l = node + 1;
    int r = node + 2 * (mid - begin);
    visit(l, begin, mid, cb);
    visit(r, mid, end, cb);
    data[node].min = ::min(data[l].min, data[r].min) + data[node].add;
  }
 
  struct Data {
    int64_t min;
    int64_t add = 0;
  };
 
  int size;
  vector<Data> data;
};
 
long long mow(int N, int C, int B, std::vector<int> &a, std::vector<int> &v) {
  assert(1 <= N && N <= kMaxN);
  assert(1 <= C && C <= kMaxC);
  assert(1 <= B && B <= kMaxB);

  assert(v.size() == N);
  assert(a.size() == N);

  vector<int64_t> A(N);
  for (int i = 0; i < N; ++i) {
    assert(1 <= a[i] && a[i] <= kMaxA);
    A[i] = a[i];
  }
 
  vector<int64_t> X(N + 1, 0);
  for (int i = 0; i < N; ++i) {
    assert(1 <= v[i] && v[i] <= kMaxV);
    X[i + 1] = v[i];
    X[i + 1] += X[i];
  }
 
  vector<int> remainders(1, 0);
  for (int i = 0; i < N; ++i)
    remainders.push_back(X[i] % C);
  ranges::sort(remainders);
  remainders.erase(unique(remainders.begin(), remainders.end()),
                   remainders.end());
 
  int K = remainders.size();
  SegmentTree S(K, numeric_limits<int64_t>::max() / 2);
  S.set(0, 0);
 
  for (int i = 0; i < N; ++i) {
    int from = ranges::lower_bound(remainders, X[i] % C) - remainders.begin();
    int to = ranges::lower_bound(remainders, X[i + 1] % C) - remainders.begin();
    S.set(from, S.min());
 
    int64_t extend = divceil(X[i + 1] - X[i], C);
 
    S.update(0, K, extend * A[i] + (extend - 1) * B);
    if (from < to) {
      S.update(from + 1, to, A[i] + B);
    } else {
      S.update(0, to, A[i] + B);
      S.update(from + 1, K, A[i] + B);
    }
    S.update(from, from + 1, B);
  }
 
  return S.min();
}