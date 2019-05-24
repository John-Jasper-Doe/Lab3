/**
 ******************************************************************************
 * @file    chunklist.hpp
 * @author  Maxim <aveter@bk.ru>
 * @date    01/05/2019
 * @brief   Description of the template "Chunk List".
 ******************************************************************************
 */


#ifndef CHUNKLIST_HPP_
#define CHUNKLIST_HPP_

#include <iostream>
#include <cstddef>
#include <cstring>


namespace {

/**
 * Discription item memory as a doubly linked list.
 *
 * @tparam T - the type of the data in the cells.
 */
template<typename T>
struct chunk
{
  chunk *next;  /**< - pointer to the next item in the list. */
  chunk *prev;  /**< - pointer to the previous item in the list. */

  T value;      /**< - cells with information */
};

} /* namespace */


/**
 * Discription structure to the allocated memory.
 *
 * This is the buffer that represents the simple doubly linked list.
 * @tparam T - the type of the data in the cells.
 * @tparam CAPACITY - number of memory cells of a given type.
 */
template<typename T, std::size_t CAPACITY>
class chunk_list
{
  public:
    /**
     * Constructor
     */
    chunk_list() {
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

    /**
     * Virtual distructor
     */
    virtual ~chunk_list() {
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


    /**
     * @brief Allocate memory for an object.
     * @return Pointer on the memory for an object.
     */
    T * alloc() {
      if (head_ == nullptr && ptr_list_ != nullptr)
        head_ = ptr_list_;
      else if (head_->next != nullptr)
        head_ = head_->next;
      else
        return nullptr;

      ++size_;
      return &head_->value;
    }

    /**
     * @brief Deallocate memory from the object.
     * @param ptr [in] - pointer to the object.
     */
    void dealloc(T *ptr) {
      if (!is_valid_addr(ptr) && ptr == nullptr)
        return;

      if (ptr == &head_->value) {
        head_ = head_->prev;
        --size_;
      }
    }

    /**
     * @brief Check on valid addres.
     * @param ptr [] - pointer to the checked address.
     * @return true is addres valid otherwise false.
     */
    bool is_valid_addr(T *ptr) {
      return (char *)ptr >= (char *)ptr_list_ &&
             (char *)ptr < (char *)(ptr_list_ + (CAPACITY * sizeof(chunk<T>)));
    }

    /**
     * @brief Memory status, full or not.
     * @return true is filled, otherwise false.
     */
    bool is_filled() {
      return size() == CAPACITY;
    }

    /**
     * @brief Size.
     * @return Occupied memory size.
     */
    std::size_t size() {
      return size_;
    }


  private:
    std::size_t size_ = 0;    /**< - the number of occupied items */
    chunk<T> *ptr_list_ =     /**< - pointer */
        static_cast<chunk<T> *>(::operator new[](CAPACITY * sizeof(chunk<T>)));
    chunk<T> *head_ = nullptr;  /**< - pointer on the head list. */
};

#endif /* CHUNKLIST_HPP_ */
