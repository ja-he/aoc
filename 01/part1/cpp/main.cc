#include <iostream>
#include <vector>
#include <string>

int
main(void)
{
  std::vector<uint32_t> values;

  uint32_t i;
  while (std::cin >> i) {
    values.push_back(i);
  }

  for (const auto& val : values) {
    if (std::find(values.begin(), values.end(), 2020-val) < values.end()) {
      std::cout << val * (2020-val) << std::endl;
      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}
