/**
 ******************************************************************************
 * @file    nodelist.h
 * @author  Maxim <aveter@bk.ru>
 * @date    01/05/2019
 * @brief   Description of the template "Node List".
 ******************************************************************************
 */

#ifndef NODELIST_H_
#define NODELIST_H_

#include <cstddef>
#include <iostream>
#include <memory>


namespace {

/**
 * Discription of the structure of the node of a singe-linked list.
 *
 * @tparam T - the type of variable stored in the node.
 */
template<typename T>
struct node {
  node *next;   /**< - next node. */
  T value;      /**< - value of the node */

  /**
   * @brief Constructor with a variable number of parameters.
   * @tparam - ...Args - constructor params.
   */
  template<typename ...Args>
  node(Args &&... args)
    : next(nullptr), value(std::forward<Args>(args)...)
  {}
};


/**
 * Discription of an iterator for working with a single-linked list.
 *
 * @tparam T - the type of variable stored in the node.
 */
template<typename T>
class node_iterator : public std::iterator<std::input_iterator_tag, T>
{
  public:
    /**
     * @brief Constructor with param.
     * @param p [in] - pointer to a single-linked list. Default p = nullptr.
     */
    node_iterator(node<T> *p = nullptr)
      : ptr_(p)
    {}

    /**
     * @brief Inequality operator.
     * @param  other [in] - iterator.
     * @return true if the iterators are not equal and false otherwise.
     */
    bool operator!=(node_iterator const &other) const {
      return ptr_ != other.ptr_;
    }

    /**
     * @brief Comparison operator.
     * @param other [in] - iterator
     * @return true if equal and false otherwise.
     */
    bool operator==(node_iterator const &other) const {
      return ptr_ == other.ptr_;
    }

    /**
     * @brief Dereference operator.
     * @return reference on the data.
     */
    const T & operator*() const {
      return ptr_->value;
    }

    /**
     * @brief Pointer selector operator.
     * @return pointer on the data.
     */
    const T * operator->() const {
      return &(ptr_->value);
    }

    /**
     * @brief Increment operator.
     * @return increment data.
     */
    node_iterator & operator++() {
      if (ptr_)
        ptr_ = ptr_->next;
      return *this;
    }


  private:
    node<T> *ptr_{nullptr};   /**< - pointer to a single-linked list. */

};

} /* namespace */





template<typename T, typename A = std::allocator<node<T>>>
class node_list
{
  public:

    using node_t = node<T>;
    using allocator_t =
            typename std::allocator_traits<A>::template rebind_alloc<node_t>;
    using iterator_t = node_iterator<T>;
    using const_iterator_t = const node_iterator<T>;

    node_list() = default;
    ~node_list() {
      while (head_) {
        node_t *next = head_->next;
        allocator.destroy(&head_->value);
        allocator.deallocate(head_, 1);
        head_ = next;
      }
    }

    /* moved */
    node_list(node_list &&other) noexcept { std::swap(this, other); }
    node_list & operator =(node_list &&other) noexcept {
      std::swap(this, other);
      return *this;
    }

    /* don't copyed */
    node_list(const node_list &) = delete;
    node_list & operator =(const node_list &) = delete;

    iterator_t begin() { return iterator_t(head_); }
    iterator_t end() { return iterator_t(); }

    const_iterator_t cbegin() const { return const_iterator_t(head_); }
    const_iterator_t cend() const { return const_iterator_t(); }

    std::size_t size() { return size_; }

    /* Push_fronts... */
    void push_front(T &value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, value});
      new_node->next = head_;
      head_ = new_node;
      ++size_;

#ifdef DEBUG_CODE
      std::cout << "void push_front(T &value)" << std::endl;
#endif /* DEBUG_CODE */
    }

    void push_front(T &&value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, std::move(value)});
      new_node->next = head_;
      head_ = new_node;
      ++size_;

#ifdef DEBUG_CODE
      std::cout << "void push_front(T &&value)" << std::endl;
#endif /* DEBUG_CODE */
    }

    template<typename... Args>
    void push_front(Args &&... args) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr,
                                           T(std::forward<Args>(args)...)});
      new_node->next = head_;
      head_ = new_node;
      ++size_;

#ifdef DEBUG_CODE
      std::cout << "void push_front(Args &&... args)" << std::endl;
#endif /* DEBUG_CODE */
    }

    /* Push_backs... */
    void push_back(T &value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, value});

      if (head_ == nullptr)
        head_ = new_node;
      else {
        node_t *next_node = head_->next;
        if (next_node == nullptr)
          head_->next = new_node;
        else {
          while (next_node->next)
            next_node = next_node->next;

          next_node->next = new_node;
        }
      }
      ++size_;
    }

    void push_back(T &&value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, std::move(value)});

      if (head_ == nullptr)
        head_ = new_node;
      else {
        node_t *next_node = head_->next;
        if (next_node == nullptr)
          head_->next = new_node;
        else {
          while (next_node->next)
            next_node = next_node->next;

          next_node->next = new_node;
        }
      }
      ++size_;
    }

    template<typename... Args>
    void push_back(Args &&... args) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr,
                                           T(std::forward<Args>(args)...)});

      if (head_ == nullptr)
        head_ = new_node;
      else {
        node_t *next_node = head_->next;
        if (next_node == nullptr)
          head_->next = new_node;
        else {
          while (next_node->next)
            next_node = next_node->next;

          next_node->next = new_node;
        }
      }
      ++size_;
    }


  private:
    std::size_t size_ = 0;
    node_t *head_ = nullptr;
    allocator_t allocator{};
};

#endif /* NODELIST_H_ */
