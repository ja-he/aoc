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

using Timestamp = unsigned;
using ID        = unsigned;
using Duration  = unsigned;

int
main(void)
{
  Timestamp earliest_departure;
  std::cin >> earliest_departure;

  std::vector<ID> buslines_in_service;
  {
    std::string id_str;
    while (std::getline(std::cin, id_str, ','))
    {
      if (id_str[0] == 'x') continue;
      buslines_in_service.push_back(std::stoi(id_str));
    }
  }

  std::vector<std::pair<Duration, ID>> wait_times_per_line;
  std::transform(buslines_in_service.begin(),
                 buslines_in_service.end(),
                 std::back_inserter(wait_times_per_line),
                 [earliest_departure](ID line) {
                   Timestamp tmp       = earliest_departure % line;
                   Duration  wait_time = line - tmp;
                   return std::pair<Duration, ID>{ wait_time, line };
                 });
  auto [wait_time, line] =
    *std::min_element(wait_times_per_line.begin(), wait_times_per_line.end());

  std::cout << "answer: " << wait_time * line << std::endl;

  return EXIT_SUCCESS;
}
