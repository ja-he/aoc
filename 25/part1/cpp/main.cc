#include <iomanip>
#include <iostream>

#include <algorithm>
#include <numeric>

#include <bitset>
#include <map>
#include <string>
#include <vector>

#include <cassert>

using Key           = uint64_t;
using SubjectNumber = uint64_t;
using Value         = uint64_t;
using LoopSize      = uint64_t;

LoopSize
get_loop_size(const Key k)
{
  LoopSize            i = 0;
  const SubjectNumber s = 7;
  Value               v = 1;
  while (v != k)
  {
    v = (v * s) % 20201227;
    i++;
  }
  return i;
}

Key
transform(const SubjectNumber n, const LoopSize s)
{
  Value v = 1;
  LoopSize i = 0;
  while (i < s)
  {
    v = (v * n) % 20201227;
    i++;
  }
  return v;
}

int
main(void)
{
  Key card_pub, door_pub;
  std::cin >> card_pub >> door_pub;

  LoopSize door_loopsize = get_loop_size(door_pub);
  LoopSize card_loopsize = get_loop_size(card_pub);

  Key encryption_key1 = transform(door_pub, card_loopsize);
  Key encryption_key2 = transform(card_pub, door_loopsize);
  assert(encryption_key1 == encryption_key2);

  std::cout << "encryption key: " << encryption_key1 << std::endl;
}
