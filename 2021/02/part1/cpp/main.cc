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

enum class Direction
{
  UP      = -1,
  FORWARD = 0,
  DOWN    = +1,
};

std::string
to_string(Direction d)
{
  switch (d)
  {
    case Direction::DOWN: return "down";
    case Direction::FORWARD: return "forward";
    case Direction::UP: return "up";
    default:
    {
      std::cerr << "unknown direction value '" << int(d) << "'" << std::endl;
      exit(-1);
    }
  }
}

Direction
from_string(const std::string& s)
{
  if (s == "down")
  {
    return Direction::DOWN;
  }
  else if (s == "up")
  {
    return Direction::UP;
  }
  else if (s == "forward")
  {
    return Direction::FORWARD;
  }
  else
  {
    throw std::invalid_argument("not a valid direction input");
  }
}

using Magnitude = int;

struct Movement
{
  Direction direction;
  Magnitude magnitude;
};

std::istream&
operator>>(std::istream& is, Movement& m)
{
  std::string direction_str;
  is >> direction_str;
  try
  {
    m.direction = from_string(direction_str);
    is >> m.magnitude;
  }
  catch (const std::invalid_argument& exception)
  {
    is.setstate(std::ios::failbit);
  }

  return is;
}

std::ostream&
operator<<(std::ostream& os, const Movement& m)
{
  os << to_string(m.direction) << ' ' << m.magnitude;

  return os;
}

using Depth = int;
using Horiz = int;
struct Position
{
  Horiz horiz = 0;
  Depth depth = 0;
  void  apply(const Movement& movement)
  {
    // (relying on up=(-1), down=(+1))
    // change depth on up or down
    auto delta_depth = int(movement.direction) * int(movement.magnitude);
    this->depth += delta_depth;

    // if no depth change then either movement is forward or magnitude is 0;
    // in either case we can simply apply the magnitude forward.
    this->horiz += (delta_depth == 0) ? movement.magnitude : 0;
  }
};
std::ostream&
operator<<(std::ostream& os, const Position& p)
{
  os << '(' << "horiz:" << p.horiz << ',' << "depth:" << p.depth << ')';
  return os;
}

class Submarine
{
public:
  void     move(const Movement& movement) { this->pos.apply(movement); }
  Position get_pos(void) const { return this->pos; };

private:
  Position pos;
};

int
main(void)
{
  // get values
  std::vector<Movement> movements;
  {
    Movement movement;
    while (std::cin >> movement) { movements.push_back(movement); }
  }

  Submarine sub;
  for (const auto& movement : movements) { sub.move(movement); }
  const auto& pos           = sub.get_pos();
  const auto [horiz, depth] = pos;

  std::cout << "final position: " << pos << std::endl;
  std::cout << "multiplied: " << (horiz * depth) << std::endl;
}
