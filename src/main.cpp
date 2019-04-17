#include "fixedallocator.h"

#include <iostream>
#include <map>

#include <stdint.h>


const size_t AMOUNT_OF_ELEMENTS = 10;

using normal_map_t = std::map<uint16_t, uint16_t>;
using fixed_map_t = std::map<uint16_t, uint16_t, std::less<uint16_t>,
  fixed_allocator<std::pair<const uint16_t, uint16_t>, AMOUNT_OF_ELEMENTS>>;

/* ------------------------------------------------------------------------- */
int factorial(int x)
{
  return x <= 1 ? 1 : x * factorial(x - 1);
}

/* ------------------------------------------------------------------------- */
void print_map(normal_map_t &map)
{
  for (auto &p: map) {
      std::cout << p.first << " " << p.second << std::endl;
  }
  std::cout << std::endl;
}


/* ------------------------------------------------------------------------- */
int main() {
  normal_map_t normal_map;
  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    normal_map[i] = factorial(i);
  }

  print_map(normal_map);

  return 0;
}
