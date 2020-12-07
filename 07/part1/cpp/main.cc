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

struct BagSpec
{
public:
  Bag                                   holder;
  std::vector<std::pair<unsigned, Bag>> held;
};

void
print_bag_hierarchy(const std::map<Bag, std::vector<Bag>>& parents,
                    const Bag&                             child,
                    const unsigned                         indent_level,
                    std::set<Bag>&                         colors)
{
  for (unsigned i = 0; i < indent_level; i++) std::cout << "  ";
  std::cout << child.attr << ' ' << child.color << std::endl;
  if (parents.contains(child))
  {
    for (const auto& parent : parents.at(child))
    {
      colors.insert(parent);
      print_bag_hierarchy(parents, parent, indent_level + 1, colors);
    }
  }
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

  std::map<Bag, std::vector<Bag>> parents;
  for (const BagSpec& s : bag_specs)
    for (const auto& [n, held_bag] : s.held)
      parents[held_bag].push_back(s.holder);

  std::cout << "parsed spec" << std::endl;

  std::set<Bag> colors;

  Bag shiny{ "shiny", "gold" };
  print_bag_hierarchy(parents, shiny, 0, colors);

  std::cout << "answer: " << colors.size() << std::endl;

  return EXIT_SUCCESS;
}
