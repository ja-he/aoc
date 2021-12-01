#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using SeatID = unsigned;
using Row    = unsigned;
using Seat   = unsigned;

SeatID
get_seat_id(const std::string& boarding_pass);

int
main(void)
{
  SeatID max = 0;

  std::string boarding_pass;
  while (std::cin >> boarding_pass)
  {
    SeatID s = get_seat_id(boarding_pass);
    max      = (s > max) ? s : max;
  }

  std::cout << max << std::endl;

  return EXIT_SUCCESS;
}

SeatID
get_seat_id(const std::string& boarding_pass)
{

  Row row = std::accumulate(boarding_pass.begin(),
                            boarding_pass.begin() + 7,
                            std::pair<Row, Row>(0, 128),
                            [](const std::pair<Row, Row>& row_bounds, char c) {
                              auto& [f, b] = row_bounds;
                              Row diff     = b - f;
                              if (c == 'F')
                                return std::pair<Row, Row>{ f, b - diff / 2 };
                              else
                                return std::pair<Row, Row>{ f + diff / 2, b };
                            })
              .first;

  Seat seat =
    std::accumulate(boarding_pass.begin() + 7,
                    boarding_pass.begin() + 7 + 3,
                    std::pair<Row, Row>(0, 8),
                    [](const std::pair<Seat, Seat>& seat_bounds, char c) {
                      auto& [l, r] = seat_bounds;
                      Seat diff    = r - l;
                      if (c == 'L')
                        return std::pair<Seat, Seat>{ l, r - diff / 2 };
                      else
                        return std::pair<Row, Row>{ l + diff / 2, r };
                    })
      .first;

  return row * 8 + seat;
}
