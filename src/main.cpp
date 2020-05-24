/**
 ******************************************************************************
 * @file    main.cpp
 * @author  Maxim <aveter@bk.ru>
 * @date    01/05/2019
 * @brief   The main project file is "Allocator".
 ******************************************************************************
 */

#include "fixedallocator.hpp"
#include "nodelist.hpp"

#include <iostream>
#include <map>
#include <stdint.h>


/**
 * @brief The foo struct to test the complex type.
 */
struct foo
{
  foo(int a, float b)
    : a(a), b(b)
  {}

  foo(const foo &) = delete;

  int a;
  float b;
};


const size_t AMOUNT_OF_ELEMENTS = 10; /**< - the number of reserved items. */

/* Aliases */
using normal_map_t = std::map<int, int>;
using fixed_map_t = std::map<int, int, std::less<int>,
               fixed_allocator<std::pair<const int, int>, AMOUNT_OF_ELEMENTS>>;
using normal_node_list_t = node_list<int>;
using fixed_node_list_t =
                      node_list<int, fixed_allocator<int, AMOUNT_OF_ELEMENTS>>;

/* for testing complex type */
using normal_node_list_ex_t = node_list<foo>;
using fixed_node_list_ex_t =
                      node_list<foo, fixed_allocator<foo, AMOUNT_OF_ELEMENTS>>;


/**
 * @brief Getting factorial from a given number.
 * @param x [in] - the number for which you need to get factorial.
 * @return factorial.
 */
int factorial(int x)
{
  return x <= 1 ? 1 : x * factorial(x - 1);
}


/**
 * @brief Displays the contents of the container "map".
 * @tparam T - the type of container item.
 * @param map [in] - the container to be printed.
 */
template<typename T>
void print_map(T &map)
{
  for (auto &p: map)
    std::cout << p.first << " " << p.second << std::endl;

  std::cout << std::endl;
}


/**
 * @brief Displays the contents of the container "list".
 * @tparam T - the type of container item.
 * @param list [in] - the container to be printed.
 */
template<typename T>
void print_list(T &list)
{
  for (auto &val: list)
    std::cout << val << std::endl;

  std::cout << std::endl;
}


/**
 * @brief Displays the contents of the container "list" with complex type.
 * @tparam T - the type of container item.
 * @param list_complex_type [in] - the container to be printed.
 */
template<typename T>
void print_list_complex_type(T &list_complex_type)
{
  for (auto &val: list_complex_type)
    std::cout << val.a << " " << val.b << std::endl;

  std::cout << std::endl;
}


/**
 * @brief Main function / entry point.
 */
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

  normal_node_list_t normal_node_list;
  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    normal_node_list.push_back(factorial(i));
  }
  print_list(normal_node_list);

  fixed_node_list_t fixed_node_list;
  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    fixed_node_list.push_back(factorial(i));
  }
  print_list(fixed_node_list);

  /* for testing push_front */
  normal_node_list_t normal_node_list2;
  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    normal_node_list2.push_front(factorial(i));
  }
  print_list(normal_node_list2);

  /* for testing complex type */
  normal_node_list_ex_t normal_node_list_ex;
  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    normal_node_list_ex.push_back(factorial(i), 2.2f);
  }
  print_list_complex_type(normal_node_list_ex);


  fixed_node_list_ex_t fixed_node_list_ex;
  for (unsigned i = 0; i < AMOUNT_OF_ELEMENTS; ++i) {
    fixed_node_list_ex.push_back(factorial(i), 2.2f);
  }
  print_list_complex_type(fixed_node_list_ex);

  return 0;
}
