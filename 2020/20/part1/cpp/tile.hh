#pragma once

#include <bitset>
#include <string>
#include <vector>

#include <cassert>

typedef std::bitset<10> Border;
typedef size_t          TileIndex;
typedef size_t          TileID;

Border
reverse(const Border& b);

/**
 * A basic tile, consisting of four borders:
 *
 *             top
 *          9        0
 *        9 #..##..### 9
 *          #        #
 *          #        .
 *          .        .
 *   left   .        #    right
 *          #        #
 *          #        #
 *          .        #
 *          .        #
 *        0 ......#### 0
 *          9        0
 *             bottom
 */
class Tile
{
public:
  Tile(std::string top_str,
       std::string bottom_str,
       std::string left_str,
       std::string right_str)
    : top(top_str, 0, 10, '.', '#')
    , bottom(bottom_str, 0, 10, '.', '#')
    , left(left_str, 0, 10, '.', '#')
    , right(right_str, 0, 10, '.', '#'){};
  Tile(const Border& top,
       const Border& bottom,
       const Border& left,
       const Border& right)
    : top{ top }
    , bottom{ bottom }
    , left{ left }
    , right{ right } {};

  const Border& get_top(void) const { return this->top; };
  const Border& get_bottom(void) const { return this->bottom; };
  const Border& get_left(void) const { return this->left; };
  const Border& get_right(void) const { return this->right; };

private:
  Border top;
  Border bottom;
  Border left;
  Border right;
};

bool
operator==(const Tile& l, const Tile& r);

/**
 * Returns a given tile flipped on the horizontal axis
 *
 * Given a Tile
 *    ABCD
 *    L  E
 *    K  F
 *    JIHG
 * flipped it is
 *    JIHG
 *    K  F
 *    L  E
 *    ABCD
 *
 * @param t the tile
 * @return t flipped
 */
Tile
flip_h(const Tile& t);

/**
 * Returns a given tile rotate clockwise by 90 degrees
 *
 * Given a Tile
 *    ABCD
 *    L  E
 *    K  F
 *    JIHG
 * rotated it is
 *    JKLA
 *    I  B
 *    H  C
 *    GFED
 *
 * @param t the tile
 * @return t flipped
 */
Tile
rotate_90(const Tile& t);

// TODO: test
std::vector<Tile>
generate_permutations(const Tile& rb);

std::ostream&
operator<<(std::ostream& os, const Tile t);

class TileTemplate
{
public:
  // TODO: test
  TileTemplate(TileID id, const Tile& base)
    : id(id)
    , permutations{ generate_permutations(base) } {};
  TileID                   get_id(void) const { return this->id; };
  const std::vector<Tile>& get_permutations(void) const
  {
    return this->permutations;
  };

private:
  TileID            id;
  std::vector<Tile> permutations;
};

std::ostream&
operator<<(std::ostream& os, const TileTemplate t);

// TODO: test (maybe, idk if feasible to test)
std::vector<TileTemplate>
read_in_tiles(std::istream&);

class TileFitting
{
public:
  bool fits(TileID a, TileID b) { return matrix[a][b]; }

private:
  std::vector<std::vector<bool>> matrix;
};

typedef uint8_t Fit;

Fit
get_fit(const Tile& l, const Tile& r);

class Transformation
{
public:
  Transformation(int index)
  {
    assert(index < 8);
    assert(index >= 0);
    this->flip          = index > 3;
    this->num_rotations = index % 4;
  }
  uint8_t num_rotations = 0;
  bool    flip          = false;
};
