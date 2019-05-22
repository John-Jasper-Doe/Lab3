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

    fixed_allocator(const fixed_allocator &) = default;
    fixed_allocator(fixed_allocator &&) noexcept = default;

    fixed_allocator &operator=(const fixed_allocator &) = default;
    fixed_allocator &operator=(fixed_allocator &&) noexcept = default;

    pointer allocate(std::size_t n) {
      pointer res = nullptr;

      if (n == 1) {
        if (!mem_chunk_.is_filed())
          res = mem_chunk_.alloc();
      }
      else
        res = reinterpret_cast<pointer>(::operator new(n * sizeof(T)));

      return res;
    }

    void deallocate(pointer p, std::size_t n) {
      if (n == 1) {
        if (mem_chunk_.is_valid_addr(p)) {
          mem_chunk_.dealloc(p);
          return;
        }
        // TODO: Added throw!!!
      }
      else
        ::operator delete(p);
    }

    template<class U, class... Args>
    void construct(U *p, Args &&... args) {
      ::new((void *) p) U(std::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U *p) {
      p->~U();
    }

   private:
    chunk_list<T, ELEMENTS> mem_chunk_;
  };

#endif  /* FIXEDALLOCATOR_H_ */
