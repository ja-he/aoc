#include <iostream>
#include <vector>
#include <set>
#include <string>

int
main(void)
{
  std::vector<uint32_t> values;
  std::set<uint32_t> doublecheck;

  uint32_t i;
  while (std::cin >> i) {
    values.push_back(i);
  }

  for (const auto& a : values) {
    for (const auto& b : values) {
      for (const auto& c : values) {
        if (a+b+c == 2020) {
          if (a == b) {
            doublecheck.insert(a);
          } else if (a == c) {
            doublecheck.insert(a);
          } else if (b == c) {
            doublecheck.insert(b);
          } else {
            std::cout << a*b*c << std::endl;
            return EXIT_SUCCESS;
          }
        }
      }
    }
  }

  for (const auto& val : doublecheck) {
    if (std::count(values.begin(), values.end(), val) > 1) {
      if (std::find(values.begin(), values.end(), 2020-(2*val)) != values.end()) {
         std::cout << val*val*(2020-(2*val)) << std::endl;
         return EXIT_SUCCESS;
      }
    }
  }

  return EXIT_FAILURE;
}
