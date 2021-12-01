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
    while (std::cin >> value) {
      values.push_back(value);
    }
  }

  int counter = 0;
  for (size_t i = 1; i < values.size(); i++) {
    if (values[i] > values[i-1]) {
      counter++;
    }
  }
  
  std::cout << "final count: " << counter << std::endl;
}
