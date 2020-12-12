#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include <cassert>

// clang-format off
#define N 0b00
#define E 0b01
#define S 0b10
#define W 0b11
#define NORTH   'N'
#define SOUTH   'S'
#define EAST    'E'
#define WEST    'W'
#define FORWARD 'F'
#define LEFT    'L'
#define RIGHT   'R'
// clang-format on

using InstructionPrefix = char;
using Direction         = uint8_t;

int
manhattan(const std::pair<int, int>& a, const std::pair<int, int>& b)
{
  auto& [ax, ay] = a;
  auto& [bx, by] = b;
  return std::abs(ax - bx) + std::abs(ay - by);
}

int
main(void)
{
  std::vector<std::string> input;
  std::string              line;
  while (std::getline(std::cin, line)) input.push_back(line);

  std::vector<std::pair<int, int>> vectors;
  Direction                        d = E;
  std::transform(input.begin(),
                 input.end(),
                 std::back_inserter(vectors),
                 [&d](const std::string& instruction) {
                   int x = 0;
                   int y = 0;

                   InstructionPrefix prefix = instruction[0];
                   int               n      = std::stoi(instruction.substr(1));

                   switch (prefix)
                   {
                     // clang-format off
                     case NORTH: { y += n; break; }
                     case SOUTH: { y -= n; break; }
                     case EAST: { x += n; break; }
                     case WEST: { x -= n; break; }
                     // clang-format on
                     case FORWARD:
                     {
                       switch (d)
                       {
                         // clang-format off
                         case N: { y+=n; break; }
                         case S: { y-=n; break; }
                         case E: { x+=n; break; }
                         case W: { x-=n; break; }
                         // clang-format on
                         default:
                         {
                           std::cerr
                             << "unexpectedly d was not a directional value: "
                             << (int)d << std::endl;
                           exit(EXIT_FAILURE);
                         }
                       }
                       break;
                     }

                     case LEFT:
                     {
                       uint8_t turn_nums = (n / 90);
                       // why this weird cast? else it don't work right...
                       d = ((Direction)(d - turn_nums)) % 4;
                       assert(d < 4);
                       break;
                     }
                     case RIGHT:
                     {
                       uint8_t turn_nums = (n / 90);
                       d                 = ((Direction)(d + turn_nums)) % 4;
                       assert(d < 4);
                       break;
                     }
                     default:
                     {
                       std::cerr << "unexpectedly read no instruction prefix\n";
                       exit(EXIT_FAILURE);
                     }
                   }

                   return std::pair<int, int>{ x, y };
                 });

  for (auto [x, y] : vectors) { std::cout << x << ',' << y << std::endl; }

  auto [x, y] = std::accumulate(
    vectors.begin(),
    vectors.end(),
    std::pair<int, int>{ 0, 0 },
    [](const std::pair<int, int>& acc, const std::pair<int, int>& v) {
      auto& [acc_x, acc_y] = acc;
      auto& [x, y]         = v;
      return std::pair<int, int>{ acc_x + x, acc_y + y };
    });

  std::cout << "distance to origin: " << std::abs(x + y) << std::endl;

  return EXIT_SUCCESS;
}
