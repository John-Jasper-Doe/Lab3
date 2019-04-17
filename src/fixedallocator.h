#ifndef FIXEDALLOCATOR_H_
#define FIXEDALLOCATOR_H_

#include "chunklist.h"

#include <cstddef>


template<typename T, std::size_t ELEMENTS>
class fixed_allocator
{
  public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;


    template<typename U>
    struct rebind {
      using other = fixed_allocator<U, ELEMENTS>;
    };

    fixed_allocator() = default;
    ~fixed_allocator() = default;

    /* moved */
    fixed_allocator(fixed_allocator &&) noexcept = default;
    fixed_allocator &operator=(fixed_allocator &&) noexcept = default;

    /* don't copyed */
    fixed_allocator(const fixed_allocator &) = delete;
    fixed_allocator &operator=(const fixed_allocator &) = delete;


    pointer allocate(std::size_t n) {

      return reinterpret_cast<pointer>(0);
    }

    void deallocate(pointer p, std::size_t n) {

    }

    template<class U, class... Args>
    void construct(U *p, Args &&... args) {

    }

    template<class U>
    void destroy(U *p) {

    }

   private:
//     std::list<fixed_size_list<T, ELEMENTS>> caches_;
  };

#endif  /* FIXEDALLOCATOR_H_ */
