#include <iomanip>
#include <iostream>

#include <algorithm>
#include <memory>
#include <numeric>

#include <bitset>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <cassert>
#include <cctype>

using Value = uint64_t;

int
main(void)
{
  // get values
  std::vector<Value> values;
  {
    Value value;
    while (std::cin >> value) { values.push_back(value); }
  }

  // sum up the values in a window
  const auto window_sum = [&values](const size_t window_end_index)
  {
    return values[window_end_index - 2] + values[window_end_index - 1] +
           values[window_end_index];
  };
  // return whether a window is bigger than the previous window
  const auto is_bigger_than_previous = [&window_sum](size_t window_end_index)
  { return window_sum(window_end_index) > window_sum(window_end_index - 1); };

  // count increases
  int counter = 0;
  for (size_t window_end_index = 3; window_end_index < values.size();
       window_end_index++)
  {
    if (is_bigger_than_previous(window_end_index)) { counter++; }
  }

  std::cout << "final count: " << counter << std::endl;
}
