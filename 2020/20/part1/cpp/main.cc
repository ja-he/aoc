#include <iostream>

#include <bitset>
#include <string>
#include <vector>

#include <algorithm>
#include <numeric>

#include <cassert>
#include <cmath>

#include "tile.hh"

int
main(void)
{
  std::vector<TileTemplate> tiles = read_in_tiles(std::cin);

  int n_tiles     = tiles.size();
  int side_length = std::sqrt(n_tiles);
  assert(side_length * side_length == n_tiles);

  std::sort(tiles.begin(),
            tiles.end(),
            [](const TileTemplate& l, const TileTemplate& r) {
              return l.get_id() < r.get_id();
            });

  // get the number of fitting pieces for each piece
  // TODO: this is doing twice as much work as necessary, no?
  std::vector<int> num_fitting_pieces(n_tiles, 0);
  for (size_t ti = 0; ti < tiles.size(); ti++)
  {
    auto& current              = tiles[ti];
    auto& current_permutations = current.get_permutations();
    for (size_t tj = 0; tj < tiles.size(); tj++)
    {
      if (ti == tj)
        continue;
      auto& neighbor              = tiles[tj];
      auto& neighbor_permutations = neighbor.get_permutations();
      bool  fit_set               = false;
      for (size_t pi = 0; pi < 8; pi++)
      {
        auto& current_perm = current_permutations[pi];
        for (size_t pj = 0; pj < 8; pj++)
        {
          auto& neighbor_perm = neighbor_permutations[pj];
          if ((!fit_set) && (get_fit(current_perm, neighbor_perm)))
          {
            fit_set = true;
            num_fitting_pieces[ti]++;
          }
        }
      }
    }
  }

  // grab probable corner tiles
  uint64_t         prod = 1;
  std::vector<int> corner_tile_indices;
  for (size_t ti = 0; ti < tiles.size(); ti++)
  {
    if (num_fitting_pieces[ti] == 2)
    {
      prod *= tiles[ti].get_id();
      corner_tile_indices.push_back(ti);
    }
  }
  assert(corner_tile_indices.size() == 4);

  std::cout << "product: " << prod << std::endl;

  return EXIT_SUCCESS;
}
