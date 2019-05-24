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


/* Forward ad */
template<typename T, std::size_t CAPACITY>
class chunk_list;

/**
 * Swap the node list.
 *
 * @tparam Tp - the type of variable stored in the node.
 * @tparam Aloc - allocator, memory manager for working with container. Default
 *                on std::allocator.
 * @param dst [in] - receiving container.
 * @param src [out] - source container.
 */
template<typename Tp, std::size_t SZ>
void swap(chunk_list<Tp, SZ> &dst, chunk_list<Tp, SZ> &src)
{
  std::swap(dst.size_, src.size_);
  std::swap(dst.ptr_list_, src.ptr_list_);
  std::swap(dst.head_, src.head_);
}

/**
 * Copying the nodes.
 *
 * @tparam Tp - the type of variable stored in the node.
 * @tparam Aloc - allocator, memory manager for working with container. Default
 *                on std::allocator.
 * @param dst [in] - reference to the source node list.
 * @param src [out] - reference to the source node list.
 */
template<typename Tp, std::size_t SZ>
void copy(chunk_list<Tp, SZ> &dst, const chunk_list<Tp, SZ> &src)
{
  dst.size_ = src.size_;
  dst.head_ = nullptr;

  if (dst.size() > 0) {
    std::size_t count = 0;
    std::size_t size = dst.size();
    chunk<Tp> *cur_other = src.ptr_list_;
    while (count < size) {
      Tp * tmp_ptr = src.alloc();
      std::memcpy(tmp_ptr, &cur_other->value, sizeof(Tp));
      dst.head_ = cur_other;
      cur_other = cur_other->next;
      ++count;
    }
  }
}


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

    /**
     * @brief Move constructor.
     * @param other [in] - the object to move.
     */
    chunk_list(chunk_list &&other) {
      swap(this, other);
    }

    /**
     * @brief Move operator.
     * @param other [in] - the object to move.
     */
    chunk_list & operator=(chunk_list &&other) {
      swap(this, other);
      return *this;
    }

    /**
     * @brief Copy constructor.
     * @param other [in] - the object to copy.
     */
    chunk_list(const chunk_list &other) {
      copy(this, other);
    }

    /**
     * @brief Copy operator.
     * @param other [in] - the object to copy.
     */
    chunk_list & operator=(const chunk_list &other) {
      copy(this, other);
      return *this;
    }

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

    /* Friends function */
    template<typename Tp, std::size_t SZ>
    friend void swap(chunk_list<Tp, SZ> &dst, chunk_list<Tp, SZ> &src);

    template<typename Tp, std::size_t SZ>
    friend void copy(chunk_list<Tp, SZ> &dst, const chunk_list<Tp, SZ> &src);
};


#endif /* CHUNKLIST_HPP_ */
