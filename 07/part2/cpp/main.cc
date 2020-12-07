#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include <cassert>

auto
tokenize(const std::string& line) -> std::vector<std::string>
{
  std::vector<std::string> r;
  std::stringstream        s(line);
  std::string              t;
  while (std::getline(s, t, ' ')) r.push_back(t);
  return r;
}

using Attribute = std::string;
using Color     = std::string;

struct Bag
{
public:
  Attribute attr;
  Color     color;
};
bool
operator<(const Bag& a, const Bag& b)
{
  return a.attr + a.color < b.attr + b.color;
}
std::ostream&
operator<<(std::ostream& os, const Bag& b)
{
  return os << '[' << b.attr << ',' << b.color << ']';
}

struct BagSpec
{
public:
  Bag                                   holder;
  std::vector<std::pair<unsigned, Bag>> held;
};

unsigned
count_bags(const std::map<Bag, std::vector<std::pair<unsigned, Bag>>>& children,
           const Bag&                                                  parent,
           unsigned                                                    n)
{
  unsigned result;
  if (children.contains(parent))
  {
    result = n + n * std::accumulate(
                       children.at(parent).begin(),
                       children.at(parent).end(),
                       0,
                       [&children](unsigned c, const auto& p) {
                         return c + count_bags(children, p.second, p.first);
                       });
  }
  else
  {
    result = n;
  }
  std::cout << "bag " << parent << " accounts for " << result << " bags."
            << std::endl;
  return result;
}

int
main(void)
{
  std::vector<std::vector<std::string>> input;

  // grab input
  std::string line;
  while (std::getline(std::cin, line)) { input.push_back(tokenize(line)); }

  std::vector<BagSpec> bag_specs;
  std::transform(
    input.begin(),
    input.end(),
    std::back_inserter(bag_specs),
    [](const auto& line) {
      BagSpec        b{ { line[0], line[1] }, {} };
      const unsigned held_part_token_count = (line.size() - 4);
      assert(held_part_token_count == 3 || held_part_token_count % 4 == 0);
      unsigned held_desc_count =
        (held_part_token_count == 3) ? 0 : (held_part_token_count / 4);

      for (unsigned i = 0; i < held_desc_count; i++)
      {
        b.held.push_back({ std::stoul(line[4 + (i * 4)]),
                           { line[4 + (i * 4) + 1], line[4 + (i * 4) + 2] } });
      }

      return b;
    });

  std::map<Bag, std::vector<std::pair<unsigned, Bag>>> children;
  for (const BagSpec& s : bag_specs)
    for (const auto& p : s.held) children[s.holder].push_back(p);

  std::cout << "parsed spec" << std::endl;

  Bag shiny{ "shiny", "gold" };

  int result = count_bags(children, shiny, 1) -1;

  std::cout << "answer: " << result << std::endl;

  return EXIT_SUCCESS;
}
