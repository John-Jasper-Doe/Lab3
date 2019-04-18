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
      std::cout << "CAPACITY = " << CAPACITY << "\n" <<
                   "Type size = " << sizeof(int) << " byte" << std::endl;
    #endif

      for (std::size_t i = 0; i < CAPACITY; ++i) {
        if (i == 0)
          ptr_list_[i].prev = nullptr;
        else
          ptr_list_[i]. prev = &ptr_list_[i - 1];

        if (i == CAPACITY - 1)
          ptr_list_[i].next = nullptr;
        else
          ptr_list_[i].next = &ptr_list_[i + 1];
      }
    }

    ~chunk_list() {
      ::operator delete[](ptr_list_);
    }

    /* moved */
    chunk_list(chunk_list &&other) noexcept
      : size_(other.size_)
      , ptr_list_(other.ptr_list_)
      , head_(other.head_) {
      other.size_ = 0;
      other.ptr_list_ = nullptr;
      other.head_ = nullptr;
    }

    chunk_list & operator =(chunk_list &&other) noexcept {
      std::swap(size_, other.size_);
      std::swap(ptr_list_, other.ptr_list_);
      std::swap(head_, other.head_);
      return *this;
    }

    /* don't copyed */
    chunk_list(const chunk_list &) = delete;
    chunk_list & operator =(const chunk_list &) = delete;

    T * alloc() {
      if (head_->next != nullptr) {
        head_ = head_->next;
        ++size_;

        return &head_->value;
      }
      else
        return nullptr;
    }

    void dealloc(T *ptr) {
      if (!is_valid_addr(ptr) && ptr == nullptr)
        return;

      if (ptr == &head_->value && head_->prev != nullptr) {
        head_ = head_->prev;
        --size_;
      }
    }

    bool is_valid_addr(T *ptr) {
      return ptr >= ptr_list_ && ptr < ptr_list_[CAPACITY * sizeof(chunk<T>)];
    }

  private:
    std::size_t size_ = 0;
    chunk<T> *ptr_list_ =
        static_cast<chunk<T> *>(::operator new[](CAPACITY * sizeof(chunk<T>)));
    chunk<T> *head_ = ptr_list_;
};

#endif /* CHUNKLIST_H_ */
