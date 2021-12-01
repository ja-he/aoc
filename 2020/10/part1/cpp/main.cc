#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include <cassert>

using Adapter = unsigned long long;

int
main(void)
{
  std::vector<Adapter> adapters;
  {
    Adapter n;
    while (std::cin >> n) { adapters.push_back(n); }
  }

  std::sort(adapters.begin(), adapters.end());

  Adapter  prev         = 0;
  unsigned n_single_gap = 0;
  unsigned n_triple_gap = 0;
  for (Adapter curr : adapters)
  {
    if (curr - prev == 1) n_single_gap++;
    if (curr - prev == 3) n_triple_gap++;
    prev = curr;
  }
  n_triple_gap += 1;

  std::cout << n_single_gap << " · " << n_triple_gap << " = " 
            << (n_single_gap * n_triple_gap) << std::endl;

  return EXIT_SUCCESS;
}
