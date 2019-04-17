#ifndef CHUNKLIST_H_
#define CHUNKLIST_H_

#include <cstddef>
#include <iostream>

#define DEBUGING

namespace {
  /**
   *
   */
  template<typename T>
  struct chunk {
    chunk *next;
    chunk *prev;

    T value;
  };
} /* namespace */


template<typename T, std::size_t CAPACITY>
class chunk_list {
  public:
    chunk_list() {
    #ifdef DEBUGING
      std::cout << "CAPACITY = " << CAPACITY <<
                   "Type size = " << sizeof(T) << std::endl;
    #endif

      for (std::size_t i = 0; i < CAPACITY; ++i) {
        start_list_[i].next = &start_list_[i + 1]
      }

    }

    ~chunk_list() {

    }

    /* moved */
    chunk_list(chunk_list &&other) noexcept {

    }

    chunk_list & operator =(chunk_list &&other) noexcept {

    }

    /* don't copyed */
    chunk_list(const chunk_list &) = delete;
    chunk_list & operator =(const chunk_list &) = delete;

  private:
    std::size_t size_ = 0;

    chunk<T> *start_list_ = static_cast<chunk<T> *>(::operator new[](CAPACITY *
                                                    sizeof(chunk<T>)));
    chunk<T> *head_ = start_list_;
};

#endif /* CHUNKLIST_H_ */
