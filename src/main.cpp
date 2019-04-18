#include "fixedallocator.h"

#include <iostream>
#include <map>

#include <stdint.h>


const size_t AMOUNT_OF_ELEMENTS = 10;

using normal_map_t = std::map<int, int>;
using fixed_map_t = std::map<int, int, std::less<int>,
  fixed_allocator<std::pair<const int, int>, AMOUNT_OF_ELEMENTS>>;

/* ------------------------------------------------------------------------- */
int factorial(int x)
{
  return x <= 1 ? 1 : x * factorial(x - 1);
}

/* ------------------------------------------------------------------------- */
template<typename T>
void print_map(T &map)
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

  fixed_map_t fixed_map;
  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    fixed_map[i] = factorial(i);
  }

  print_map(fixed_map);
  return 0;
}
