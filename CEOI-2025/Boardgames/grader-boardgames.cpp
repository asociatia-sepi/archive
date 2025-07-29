#include "boardgames.h"

#include <cassert>
#include <cstdio>
#include <vector>

// BEGIN SECRET
#include <string>
// END SECRET

int main() {
  assert(freopen("boardgames.in", "r", stdin));
  assert(freopen("boardgames.out", "w", stdout));
  // BEGIN SECRET
  const std::string input_secret = "238a335d4bbc3bc94538b3fabfff7a30";
  const std::string output_secret = "62580a764a16d5bed4152a75ce105706";
  char secret[1000];
  assert(1 == scanf("%999s", secret));
  if (std::string(secret) != input_secret) {
    printf("%s\n", output_secret.c_str());
    printf("PV\n");
    printf("Possible tampering with the input\n");
    fclose(stdout);
    return 0;
  }
  // END SECRET
  int N, M;
  assert(2 == scanf("%d %d", &N, &M));

  std::vector<int> X(M), Y(M);

  for (int i = 0; i < M; i++) {
    assert(2 == scanf("%d %d", &X[i], &Y[i]));
  }
  fclose(stdin);

  std::vector<int> K = partition_players(N, M, X, Y);

  // BEGIN SECRET
  printf("%s\n", output_secret.c_str());
  printf("OK\n");
  // END SECRET
  int g = static_cast<int>(K.size());
  printf("%d\n", g);
  for (int i = 0; i < g; i++) {
    printf(i == 0 ? "%d" : " %d", K[i]);
  }
  printf("\n");
  fclose(stdout);
  return 0;
}
