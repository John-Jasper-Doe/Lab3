/**
 ******************************************************************************
 * @file    fixedallocator.hpp
 * @author  Maxim <aveter@bk.ru>
 * @date    01/05/2019
 * @brief   Description of the template "Fixed Allocator".
 ******************************************************************************
 */

#ifndef FIXEDALLOCATOR_HPP_
#define FIXEDALLOCATOR_HPP_

#include "chunklist.hpp"

#include <cstddef>

/**
 * Discription of the "Fixed Allocator" class.
 *
 * This class will allow you to pre-allocate memory for data in such containers
 * that do not have to do this using standart methods.
 * @tparam T - data types.
 * @tparam ELEMENTS - the size of memory to reserv.
 */
template<typename T, std::size_t ELEMENTS>
class fixed_allocator
{
  public:
    /* Aliases */
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;


    template<typename U>
    struct rebind {
      using other = fixed_allocator<U, ELEMENTS>;
    };

    /* By default ... */
    fixed_allocator() = default;
    ~fixed_allocator() = default;

    fixed_allocator(const fixed_allocator &) = default;
    fixed_allocator(fixed_allocator &&) = default;

    fixed_allocator &operator=(const fixed_allocator &) = default;
    fixed_allocator &operator=(fixed_allocator &&) = default;

    /**
     * @brief allocation of a given "piece" of memory.
     * @param n [in] - amount of memory requested.
     * @return pointer to the allocated memory.
     */
    pointer allocate(std::size_t n) {
      pointer res = nullptr;

      if (n == 1) {
        if (!mem_chunk_.is_filled()) {
          res = mem_chunk_.alloc();
        }
      }
      else
        res = reinterpret_cast<pointer>(::operator new(n * sizeof(T)));

      return res;
    }

    /**
     * @brief Release a specified amount of memory.
     * @param p [in] - pointer to the beginning of the memory.
     * @param n [in] - size of free memory.
     */
    void deallocate(pointer p, std::size_t n) {
      if (n == 1) {
        if (mem_chunk_.is_valid_addr(p)) {
          mem_chunk_.dealloc(p);
          return;
        }
        /* TODO: Added throw!!! */
      }
      else
        ::operator delete(p);
    }

    /**
     * @brief Object construction.
     * @tparam U - type of object constructed.
     * @tparam Args - constructor parameters.
     * @param p [in] - pointer of the object.
     * @param args [in] - input parameters of the constructor.
     */
    template<class U, class... Args>
    void construct(U *p, Args &&... args) {
      ::new((void *) p) U(std::forward<Args>(args)...);
    }

    /**
     * @brief Object distruction.
     * @tparam U - type of object distroy.
     * @param p [in] - pointer of the object.
     */
    template<class U>
    void destroy(U *p) {
      p->~U();
    }


   private:
    chunk_list<T, ELEMENTS> mem_chunk_; /**< - structure to the allocated
                                               memory - buffer. */
  };

#endif  /* FIXEDALLOCATOR_HPP_ */
