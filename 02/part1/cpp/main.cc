#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

std::pair<uint32_t, uint32_t>
parse_numrange(const std::string& numrange);

int
main(void)
{
  unsigned valid_password_counter = 0;

  std::string numrange;
  char        letter;
  std::string password;
  while (std::cin >> numrange >> letter) {
    std::cin.ignore(2);
    std::cin >> password;
    auto [at_least, at_most] = parse_numrange(numrange);

    unsigned count = std::count(password.begin(), password.end(), letter);

    if (count >= at_least && count <= at_most) {
      valid_password_counter++;
    }
  }

  std::cout << valid_password_counter << std::endl;

  return EXIT_SUCCESS;
}

std::pair<uint32_t, uint32_t>
parse_numrange(const std::string& numrange)
{
  std::string delim = "-";
  size_t      index = numrange.find(delim);

  return { std::stoul(numrange.substr(0, index)),
           std::stoul(numrange.substr(index + 1, numrange.length() - 1)) };
}
