#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <numeric>

#include "catch2/catch.hpp"

#include "tile.hh"

TEST_CASE("top border tile construction from string", "[Tile]")
{
  Tile t1{ "..........", "##########", ".#########", ".#########" };
  REQUIRE(t1.get_top().none());
  Tile t2{ "##########", "..........", "#.........", "#........." };
  REQUIRE(t2.get_top().all());
  Tile t3{ "........#.", "..........", "..........", ".........." };
  REQUIRE(t3.get_top() == Border("0000000010"));
  REQUIRE(!t3.get_top()[8]);
  REQUIRE(t3.get_top()[1]);
}

TEST_CASE("bottom border tile construction from string", "[Tile]")
{
  Tile t1{ "##########", "..........", "#########.", "#########." };
  REQUIRE(t1.get_bottom().none());
  Tile t2{ "..........", "##########", ".........#", ".........#" };
  REQUIRE(t2.get_bottom().all());
  Tile t3{ "..........", "........#.", "..........", ".........." };
  REQUIRE(t3.get_bottom() == Border("0000000010"));
  REQUIRE(!t3.get_bottom()[8]);
  REQUIRE(t3.get_bottom()[1]);
}

TEST_CASE("left border tile construction from string", "[Tile]")
{
  Tile t1{ ".#########", ".#########", "..........", "##########" };
  REQUIRE(t1.get_left().none());
  Tile t2{ "#.........", "#.........", "##########", ".........#" };
  REQUIRE(t2.get_left().all());
  Tile t3{ "..........", "..........", "........#.", ".........." };
  REQUIRE(t3.get_left() == Border("0000000010"));
  REQUIRE(!t3.get_left()[8]);
  REQUIRE(t3.get_left()[1]);
}

TEST_CASE("right border tile construction from string", "[Tile]")
{
  Tile t1{ "#########.", "#########.", "##########", ".........." };
  REQUIRE(t1.get_right().none());
  Tile t2{ ".........#", ".........#", "..........", "##########" };
  REQUIRE(t2.get_right().all());
  Tile t3{ "..........", "..........", "..........", "........#." };
  REQUIRE(t3.get_right() == Border("0000000010"));
  REQUIRE(!t3.get_right()[8]);
  REQUIRE(t3.get_right()[1]);
}

TEST_CASE("flipping (horizontally)", "flip_h")
{
  Tile on_top{ "##########", "..........", "#.........", "#........." };
  Tile on_bottom{ "..........", "##########", ".........#", ".........#" };
  REQUIRE(flip_h(on_top) == on_bottom);
  REQUIRE(flip_h(on_bottom) == on_top);
}

TEST_CASE("rotating 90 degrees clockwise", "rotate_90")
{
  Tile on_top{ "##########", "..........", "#.........", "#........." };
  Tile on_bottom{ "..........", "##########", ".........#", ".........#" };
  Tile on_left{ "#.........", "#.........", "##########", ".........." };
  Tile on_right{ ".........#", ".........#", "..........", "##########" };
  REQUIRE(rotate_90(on_top) == on_right);
  REQUIRE(rotate_90(on_right) == on_bottom);
  REQUIRE(rotate_90(on_bottom) == on_left);
  REQUIRE(rotate_90(on_left) == on_top);
}
