#include "fixedallocator.h"
#include "nodelist.h"

#include <iostream>
#include <map>

#include <stdint.h>


const size_t AMOUNT_OF_ELEMENTS = 10;

using normal_map_t = std::map<int, int>;
using fixed_map_t = std::map<int, int, std::less<int>,
               fixed_allocator<std::pair<const int, int>, AMOUNT_OF_ELEMENTS>>;

using normal_node_list_t = node_list<int>;
using fixed_node_list_t =
                      node_list<int, fixed_allocator<int, AMOUNT_OF_ELEMENTS>>;

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
template<typename T>
void print_list(T &list)
{
  for (auto &val: list) {
      std::cout << val << std::endl;
  }
  std::cout << std::endl;
}

/* ------------------------------------------------------------------------- */
int main() {
  normal_map_t normal_map;
  fixed_map_t fixed_map;
  normal_node_list_t normal_node_list;


  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    normal_map[i] = factorial(i);
    fixed_map[i] = factorial(i);
    normal_node_list.push_back(factorial(i));
  }

  print_map(normal_map);
  print_map(fixed_map);
  print_list(normal_node_list);

  return 0;
}
