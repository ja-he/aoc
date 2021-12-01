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

// -----------------------------------------------------------------------------
// declarations
// -----------------------------------------------------------------------------

#define EMPTY 'L'
#define OCCUPIED '#'
#define FLOOR '.'

class SeatLayout
{
public:
  SeatLayout(std::istream& input);
  SeatLayout(std::string input);
  void iterate(void);
  bool has_changed(void) const;

  void print(void) const;

  size_t n_occupied_seats(void) const;
  size_t n_occupied_neighbors(const size_t i, const size_t j) const;
  size_t n_occupied_seats_in_range(const size_t i_low,
                                   const size_t j_low,
                                   const size_t i_bound,
                                   const size_t j_bound) const;

  std::pair<size_t, size_t> get_shape(void) const;
  const std::string&        get_row(size_t index) const;

private:
  std::vector<std::string> layout;
  bool                     change = true;

  void read_in_layout(std::istream& input);
};

bool
test(void);

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------

int
main(void)
{
  if (!test()) return EXIT_FAILURE;

  // parse input into seat layout data type
  SeatLayout layout(std::cin);

  size_t iter_count = 0;
  while (layout.has_changed())
  {
    layout.iterate();
    iter_count++;
  }
  auto n_occupied = layout.n_occupied_seats();
  std::cout << std::endl << "N occupied: " << n_occupied << std::endl;

  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// implementations
// -----------------------------------------------------------------------------

void
SeatLayout::print(void) const
{
  for (const auto& row : this->layout) std::cout << row << std::endl;
}

std::pair<size_t, size_t>
SeatLayout::get_shape(void) const
{
  if (this->layout.empty())
    return { 0, 0 };
  else
    return { this->layout.size(), this->layout[0].size() };
}

const std::string&
SeatLayout::get_row(size_t index) const
{
  return this->layout.at(index);
}

bool
SeatLayout::has_changed(void) const
{
  return this->change;
}

void
SeatLayout::iterate(void)
{
  this->change                        = false;
  std::vector<std::string> new_layout = this->layout;

  auto [n_rows, n_cols] = this->get_shape();
  for (size_t i = 0; i < n_rows; i++)
  {
    for (size_t j = 0; j < n_cols; j++)
    {
      if (this->layout[i][j] == EMPTY)
      {
        if (this->n_occupied_neighbors(i, j) == 0)
        {
          new_layout[i][j] = OCCUPIED;
          this->change     = true;
        }
      }
      else if (this->layout[i][j] == OCCUPIED)
      {
        if (this->n_occupied_neighbors(i, j) >= 4)
        {
          new_layout[i][j] = EMPTY;
          this->change     = true;
        }
      }
    }
  }

  this->layout = new_layout;
}

size_t
SeatLayout::n_occupied_neighbors(const size_t i, const size_t j) const
{
  auto [n_rows, n_cols] = this->get_shape();
  size_t count =
    this->n_occupied_seats_in_range((i == 0) ? i : i - 1,
                                    (j == 0) ? j : j - 1,
                                    (i == n_rows - 1) ? i + 1 : i + 2,
                                    (j == n_cols - 1) ? j + 1 : j + 2);
  if (this->layout[i][j] == OCCUPIED) count--;
  return count;
}

size_t
SeatLayout::n_occupied_seats_in_range(const size_t i_low,
                                      const size_t j_low,
                                      const size_t i_bound,
                                      const size_t j_bound) const
{
  size_t count = 0;
  for (size_t i = i_low; i < i_bound; i++)
  {
    for (size_t j = j_low; j < j_bound; j++)
    {
      if (this->layout.at(i).at(j) == OCCUPIED) count++;
    }
  }
  return count;
}

size_t
SeatLayout::n_occupied_seats(void) const
{
  auto [n_rows, n_cols] = this->get_shape();
  return this->n_occupied_seats_in_range(0, 0, n_rows, n_cols);
}

SeatLayout::SeatLayout(std::string input)
{
  std::stringstream s(input);
  this->read_in_layout(s);
}

SeatLayout::SeatLayout(std::istream& input)
{
  this->read_in_layout(input);
}

void
SeatLayout::read_in_layout(std::istream& input)
{
  this->layout = {};
  std::string row;
  while (std::getline(input, row)) { this->layout.push_back(row); }
}

bool
operator==(const SeatLayout& a, const SeatLayout& b)
{
  if (a.get_shape() != b.get_shape()) return false;
  auto [n_rows, _] = a.get_shape();

  for (size_t i = 0; i < n_rows; i++)
    if (a.get_row(i) != b.get_row(i)) return false;

  return true;
}

// -----------------------------------------------------------------------------
// testing
// -----------------------------------------------------------------------------

const std::string no_seats = "...\n"
                             "...\n"
                             "...\n";
const std::string all_empty = "LLL\n"
                              "LLL\n"
                              "LLL\n";
const std::string all_occupied = "###\n"
                                 "###\n"
                                 "###\n";
const std::string all_occ_next = "#L#\n"
                                 "LLL\n"
                                 "#L#\n";

bool
test(void)
{
  { // counts are correct
    const SeatLayout all_occ(all_occupied);
    assert(all_occ.n_occupied_seats() == 9);

    for (size_t i = 0; i < 3; i++)
      for (size_t j = 0; j < 3; j++)
        assert(all_occ.n_occupied_seats_in_range(i, j, 3, 3) ==
               (3 - i) * (3 - j));
    for (size_t i = 3; 0 < i; i--)
      for (size_t j = 3; 0 < j; j--)
        assert(all_occ.n_occupied_seats_in_range(0, 0, i, j) == i * j);

    assert(all_occ.n_occupied_neighbors(0, 0) == 3);
    assert(all_occ.n_occupied_neighbors(0, 1) == 5);
    assert(all_occ.n_occupied_neighbors(0, 2) == 3);

    assert(all_occ.n_occupied_neighbors(0, 0) == 3);
    assert(all_occ.n_occupied_neighbors(1, 0) == 5);
    assert(all_occ.n_occupied_neighbors(2, 0) == 3);

    assert(all_occ.n_occupied_neighbors(1, 1) == 8);
  }

  { // iterating on no seats doesn't change anything
    SeatLayout       l(no_seats);
    const SeatLayout test = l;

    l.iterate();

    assert(!l.has_changed());
    assert(l == test);
  }

  { // iterating on all empty seats changes them
    SeatLayout       l(all_empty);
    const SeatLayout occupied(all_occupied);
    const SeatLayout occupied_next(all_occ_next);

    l.iterate();

    assert(l.has_changed());
    assert(l == occupied);

    // ...and iterating that then leaves only the corners occupied
    l.iterate();

    assert(l.has_changed());
    assert(l == occupied_next);
  }

  { // single row (uneven input shape)
    SeatLayout l("LLLLL");
    l.iterate();
    assert(std::all_of(l.get_row(0).begin(), l.get_row(0).end(), [](char c) {
      return c == OCCUPIED;
    }));
  }

  { // test a slice of the actual input
    SeatLayout l(".LLLLLLLL.LLLL.LLL.LLLLLLL.LLLLLLLLLL\n"
                 "LLLLLL.LL.LLLLLLLLL.LLLLLL.LLLLLLLLLL\n"
                 "LLLLLLLLL.LLLLLLLLL.LLLLLL.LLLL.LLLLL\n"
                 "LLLLLL.LLLLLLLLLLLL.LLLLLL.LLLL.LLLLL\n"
                 "LLLLLLLLL.LLLLLLLL..LLLLLLLLLLLLL.LLL\n"
                 ".LLLLL.LL.LLLLLLLLL..LLLLLLLLLLLLLLLL\n"
                 "...LLLL..L.....L.L.L.....L.LLLL.....L\n"
                 "LLL.LLLLL.LLLLLLLLL.LLLLLLLLLL.LLLLLL\n"
                 "LLL.LLLLL.LLLLLLLLL.LLLLLL.LLLLL.L.LL\n"
                 "L.LLLLLLLLLLLLLLLL.LLLLLLLL.LLLLLLLLL\n"
                 "LLLLLLLLL.LLLLLLLLLLLLLLLL.LLLLLLLLLL\n"
                 ".LLLLLLL..LLLLLLLLL.LLLLLL.LLLLLLLLLL\n"
                 "LLLLLLLLLLLLLLLLLLL.LLLLLL.LLLLLLLLLL\n"
                 "LLLLLLLLL.LLLLLLLLL.LLLLLLLLLLLLLLLLL\n"
                 "LLLLL.LLL.LLLLLLLLL.L.LLLL.LLLL.LLLLL\n"
                 "L.LLLLLLLLLLLLLLLL..LLLLLL.LLLLLLLLLL\n"
                 "LL...L....L.L.L......L..L.L.LL..LLLLL\n");
    l.iterate();
    assert(l.get_row(0)[1] == '#');
    assert(l.get_row(0)[2] == '#');
    assert(l.get_row(0).back() == '#');
  }

  return true;
}
