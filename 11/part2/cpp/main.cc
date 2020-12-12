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
  size_t n_occupied_visible_from(const size_t i, const size_t j) const;
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

// -----------------------------------------------------------------------------
// main
// -----------------------------------------------------------------------------

int
main(void)
{
  // parse input into seat layout data type
  SeatLayout layout(std::cin);

  size_t iter_count = 0;
  while (layout.has_changed())
  {
    std::cerr << "iterating " << iter_count << '\n';
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
      size_t n_vis = this->n_occupied_visible_from(i, j);
      if (this->layout[i][j] == EMPTY)
      {
        if (n_vis == 0)
        {
          new_layout[i][j] = OCCUPIED;
          this->change     = true;
        }
      }
      else if (this->layout[i][j] == OCCUPIED)
      {
        if (n_vis >= 5)
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
SeatLayout::n_occupied_visible_from(const size_t i_seat,
                                    const size_t j_seat) const
{
  auto [n_rows, n_cols] = this->get_shape();
  size_t count          = 0;

  std::set<std::pair<int, int>> incrementers = {
    { 1, 0 },  // down
    { 0, 1 },  // right
    { -1, 0 }, // up
    { 0, -1 }, // left
    { 1, 1 },  // down right
    { 1, -1 }, // down left
    { -1, 1 }, // up right
    { -1, -1 } // up left
  };

  if (i_seat == 0)
  {
    incrementers.erase({ -1, 0 });
    incrementers.erase({ -1, -1 });
    incrementers.erase({ -1, 1 });
  }
  if (j_seat == 0)
  {
    incrementers.erase({ 0, -1 });
    incrementers.erase({ -1, -1 });
    incrementers.erase({ 1, -1 });
  }
  if (i_seat == n_rows - 1)
  {
    incrementers.erase({ 1, 0 });
    incrementers.erase({ 1, -1 });
    incrementers.erase({ 1, 1 });
  }
  if (j_seat == n_cols - 1)
  {
    incrementers.erase({ 0, 1 });
    incrementers.erase({ -1, 1 });
    incrementers.erase({ 1, 1 });
  }

  for (auto [i_inc, j_inc] : incrementers)
  {
    int i = i_seat + i_inc;
    int j = j_seat + j_inc;

    bool chair_found = false;
    while (!chair_found && i >= 0 && i < (int)n_rows && j >= 0 && j < (int)n_cols)
    {
      char other_seat = this->layout[i][j];
      if (other_seat == OCCUPIED || other_seat == EMPTY)
      {
        if (other_seat == OCCUPIED) count++;
        chair_found = true;
      }
      i += i_inc;
      j += j_inc;
    }
  }

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
