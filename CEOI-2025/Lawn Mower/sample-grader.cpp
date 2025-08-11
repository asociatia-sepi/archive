#include <iostream>
#include <vector>

#include "lawn.h"

/**
 * Reads input from stdin in the following format:
 *
 * n c b
 * a[0] a[1] a[2] ... a[n - 1]
 * v[0] v[1] v[2] ... v[n - 1]
 */

int main() {
  int n, c, b;
  std::cin >> n >> c >> b;

  std::vector<int> a(n);
  std::vector<int> v(n);

  for (int i = 0; i < n; i++)
    std::cin >> a[i];
  for (int i = 0; i < n; i++)
    std::cin >> v[i];

  std::cout << mow(n, c, b, a, v) << std::endl;

  return 0;
}
