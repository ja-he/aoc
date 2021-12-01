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

#define NUMRANGE 25

using Value = long long;

auto
find_error(std::vector<Value>& xmas)
{
  auto  error_pos = xmas.begin();
  for (auto it = xmas.begin() + NUMRANGE; it < xmas.end(); it++)
  {
    Value           n = *it;
    std::set<Value> sums;
    for (auto it_i = it - NUMRANGE; it_i < it; it_i++)
      for (auto it_j = it - NUMRANGE; it_j < it; it_j++)
        if (*it_i != *it_j) sums.insert(*it_i + *it_j);
    if (!sums.contains(n))
    {
      error_pos = it;
      break;
    }
  }

  if (error_pos == xmas.begin())
  {
    std::cerr << "error unexpectedly not found" << std::endl;
    exit(EXIT_FAILURE);
  }

  return error_pos;
}

auto
find_weakness(std::vector<Value>& xmas, auto error_pos)
{
  auto error = *error_pos;

  auto lb = xmas.begin();
  auto ub = xmas.begin() + 2;
  while (ub != xmas.end())
  {
    if (ub == error_pos)
    {
      lb = error_pos + 1;
      ub = error_pos + 1 + 2;
      continue;
    }

    auto sum = std::accumulate(lb, ub, 0);

    if (sum < error) { ub++; }
    else if (sum > error)
    {
      lb++;
    }
    else
    {
      // contiguous sequence found
      break;
    }
  }

  return *std::min_element(lb, ub) + *std::max_element(lb, ub);
}

int
main(void)
{

  std::vector<Value> xmas;
  {
    Value n;
    while (std::cin >> n) { xmas.push_back(n); }
  }

  auto error_pos = find_error(xmas);
  auto weakness  = find_weakness(xmas, error_pos);

  std::cout << "xmas weakness: " << weakness << std::endl;

  return EXIT_SUCCESS;
}
