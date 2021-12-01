#include "tile.hh"

#include <cassert>
#include <iostream>
#include <regex>
#include <string>

Border
reverse(const Border& b)
{
  Border rev;
  for (size_t i = 0; i < 10; i++) { rev[(10 - 1) - i] = b[i]; }
  return rev;
}

Tile
flip_h(const Tile& t)
{
  return {
    t.get_bottom(), t.get_top(), reverse(t.get_left()), reverse(t.get_right())
  };
}

Tile
rotate_90(const Tile& t)
{
  return {
    reverse(t.get_left()), reverse(t.get_right()), t.get_bottom(), t.get_top()
  };
}

std::vector<Tile>
generate_permutations(const Tile& rb)
{

  Tile r1 = rotate_90(rb);
  Tile r2 = rotate_90(r1);
  Tile r3 = rotate_90(r2);

  Tile fb = flip_h(rb);
  Tile f1 = rotate_90(fb);
  Tile f2 = rotate_90(f1);
  Tile f3 = rotate_90(f2);

  return { rb, r1, r2, r3, fb, f1, f2, f3 };
}

std::ostream&
operator<<(std::ostream& os, const Tile t)
{
  os << t.get_top().to_string('.', '#') << '\n';
  for (int i = 10 - 2; i > 0; i--)
  {
    os << (t.get_left()[i] ? '#' : '.') << std::string(10 - 2, ' ')
       << (t.get_right()[i] ? '#' : '.') << '\n';
  }
  os << t.get_bottom().to_string('.', '#') << '\n';
  return os;
}

std::ostream&
operator<<(std::ostream& os, const TileTemplate t)
{
  os << "Tile " << t.get_id() << ":\n";
  for (const auto& p : t.get_permutations()) os << p;
  os << '\n';
  return os;
}

bool
operator==(const Tile& l, const Tile& r)
{
  return l.get_top() == r.get_top() && l.get_bottom() == r.get_bottom() &&
         l.get_left() == r.get_left() && l.get_right() == r.get_right();
}

std::vector<TileTemplate>
read_in_tiles(std::istream& is)
{
  std::vector<TileTemplate> tiles;

  std::string line;
  std::string top;
  std::string bottom;
  std::string left(10, '\0');
  std::string right(10, '\0');

  TileID id = 0;

  int i = 0;
  while (std::getline(is, line))
  {
    if (line.length() == 0)
    {
      continue;
    }
    std::smatch id_match;
    if (std::regex_search(line, id_match, std::regex("Tile ([0-9]+):")))
    {
      id = std::stoul(id_match[1]);
      continue;
    }
    left[i]  = line[0];
    right[i] = line[9];
    if (i == 0)
    {
      top = line;
    }
    else if (i == 9)
    {
      bottom = line;
      assert(id != 0);
      assert(std::regex_match(top, std::regex("[#.]{10}")));
      tiles.push_back({ id, { top, bottom, left, right } });
      id = 0;
    }
    i = (i + 1) % 10;
  }
  return tiles;
}

Fit
get_fit(const Tile& l, const Tile& r)
{
  Fit f = 0;
  if (l.get_right() == r.get_left())
    f |= 0b00000001;
  if (l.get_bottom() == r.get_top())
    f |= 0b00000010;
  if (l.get_left() == r.get_right())
    f |= 0b00000100;
  if (l.get_top() == r.get_bottom())
    f |= 0b00001000;
  return f;
}
