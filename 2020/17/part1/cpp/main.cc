#include <iomanip>
#include <iostream>

#include <algorithm>
#include <numeric>

#include <bitset>
#include <map>
#include <string>
#include <vector>

#include <cassert>

#define STEP_COUNT 6
#define INIT_SIZE 8

unsigned
count_active_neigbors(
  const std::vector<std::vector<std::bitset<STEP_COUNT * 2 + INIT_SIZE>>>& grid,
  size_t                                                                   z,
  size_t                                                                   y,
  size_t                                                                   x)
{
  unsigned count = 0;
  for (auto zi = ((z == 0) ? 0 : z - 1);
       zi <= ((z == grid.size() - 1) ? grid.size() - 1 : z + 1);
       zi++)
  {
    for (auto yi = ((y == 0) ? 0 : y - 1);
         yi <= ((y == grid.size() - 1) ? grid.size() - 1 : y + 1);
         yi++)
    {
      for (auto xi = ((x == 0) ? 0 : x - 1);
           xi <= ((x == grid.size() - 1) ? grid.size() - 1 : x + 1);
           xi++)
      {
        if (grid[zi][yi][xi] && (zi != z || yi != y || xi != x)) { count++; }
      }
    }
  }
  return count;
}

unsigned
count_actives(
  const std::vector<std::vector<std::bitset<STEP_COUNT * 2 + INIT_SIZE>>>& grid)
{
  return std::accumulate(
    grid.begin(),
    grid.end(),
    0,
    [](auto                                                        count,
       const std::vector<std::bitset<STEP_COUNT * 2 + INIT_SIZE>>& slab) {
      return count + std::accumulate(
                       slab.begin(),
                       slab.end(),
                       0,
                       [](auto                                           count,
                          const std::bitset<STEP_COUNT * 2 + INIT_SIZE>& line) {
                         return count + line.count();
                       });
    });
}

std::vector<std::vector<std::bitset<STEP_COUNT * 2 + INIT_SIZE>>>
cycle_step(
  std::vector<std::vector<std::bitset<STEP_COUNT * 2 + INIT_SIZE>>>& grid)
{
  auto next_grid = grid;

  for (auto z = 0; z < STEP_COUNT * 2 + INIT_SIZE; z++)
  {
    for (auto y = 0; y < STEP_COUNT * 2 + INIT_SIZE; y++)
    {
      for (auto x = 0; x < STEP_COUNT * 2 + INIT_SIZE; x++)
      {
        auto nc = count_active_neigbors(grid, z, y, x);
        if (grid[z][y][x])
        {
          if (nc != 2 && nc != 3) { next_grid[z][y][x] = false; }
        }
        else
        {
          if (nc == 3) { next_grid[z][y][x] = true; }
        }
      }
    }
  }

  return next_grid;
}

int
main(void)
{
  std::vector<std::vector<std::bitset<STEP_COUNT * 2 + INIT_SIZE>>> grid;

  {
    for (int i = 0; i < STEP_COUNT * 2 + INIT_SIZE; i++)
      grid.push_back(std::vector<std::bitset<STEP_COUNT * 2 + INIT_SIZE>>(
        2 * STEP_COUNT + INIT_SIZE));

    int         z = STEP_COUNT + INIT_SIZE / 2;
    int         y = STEP_COUNT;
    std::string line;
    while (std::getline(std::cin, line))
    {
      std::string padded =
        std::string(STEP_COUNT, '.') + line + std::string(STEP_COUNT, '.');
      grid[z][y] = std::bitset<STEP_COUNT * 2 + INIT_SIZE>(
        padded, 0, STEP_COUNT * 2 + INIT_SIZE, '.', '#');
      y++;
    }
  }

  std::cout << "active: " << count_actives(grid) << std::endl;

  for (int i = 0; i < STEP_COUNT; i++)
  {
    grid = cycle_step(grid);
    std::cout << "now active(" << i << "): " << count_actives(grid)
              << std::endl;
  }
}
