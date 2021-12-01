#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

int
main(void)
{
  std::unordered_set<uint32_t> values;

  uint32_t i;
  while (std::cin >> i) {
    values.insert(i);
  }

  for (const auto& val : values) {
    if (values.find(2020-val) != values.end()) {
      if (val != 1010) {
        std::cout << val * (2020-val) << std::endl;
        return EXIT_SUCCESS;
      }
    }
  }

  if (values.count(1010) > 1) {
    std::cout << 1010*1010 << std::endl;
    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}
