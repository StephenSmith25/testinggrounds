#include <algorithm>
#include <cmath>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <tbb/parallel_for.h>
#include <tuple>
#include <vector>

struct NthCoefficent {
  int remainder;
  int a;
};

int nChoosek(int n, int k) {
  if (k > n)
    return 0;
  if (k * 2 > n)
    k = n - k;
  if (k == 0)
    return 1;

  int result = n;
  for (int i = 2; i <= k; ++i) {
    result *= (n - i + 1);
    result /= i;
  }
  return result;
}

NthCoefficent
getCoefficent(int N, int k) {
  int a = k - 1;
  int nCa = 0;
  if (nCa >= N) {
    return {N - nCa, a};
  }
  bool found = false;
  int aPrev = a;
  int nCaPrev = nCa;

  while (!found) {
    aPrev = a;
    nCaPrev = nCa;
    a = aPrev + 1;
    nCa = nChoosek(a, k);
    if (nCa > N)
      found = true;
  }
  return {N - nCaPrev, aPrev};
}

std::tuple<int, int> pairwiseIndex(int n)
{
  auto a2 = getCoefficent(n, 2);
  auto a1 = getCoefficent(a2.remainder, 1);
  return {a1.a, a2.a};
}

// just some practice computations
double calculateDistance(int a, int b) {
  double a1 = 1.001 * a;
  double a2 = 1.5151 * b;
  return std::sqrt((a1 - a2) * (a1 - a2));
}

int main()
{

  int N = 1000000;
  int numItems = 1000000;
  std::vector<int> vec(N);
  std::iota(vec.begin(), vec.end(), 0);

  tbb::parallel_for(tbb::blocked_range<int>(0, vec.size()),
                    [&](tbb::blocked_range<int> r) {
                      for (int i = r.begin(); i < r.end(); ++i) {
                        auto [a1, a2] = pairwiseIndex(i);
                        int f = calculateDistance(vec[a1], vec[a2]);
                      }
                    });
  return 0;
}
