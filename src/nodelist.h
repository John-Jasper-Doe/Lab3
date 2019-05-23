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


/* Forward ad */
template<typename T, typename A>
class node_list;

/**
 * Swap the node list.
 *
 * @tparam Tp - the type of variable stored in the node.
 * @tparam Aloc - allocator, memory manager for working with container. Default
 *                on std::allocator.
 * @param dst [in] - receiving container.
 * @param src [out] - source container.
 */
template<typename Tp, typename Aloc = std::allocator<node<Tp>>>
void swap(node_list<Tp, Aloc> &dst, node_list<Tp, Aloc> &src)
{
  std::swap(dst.head_, src.head_);
  std::swap(dst.allocator, src.allocator);
  std::swap(dst.size_, src.size_);
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
template<typename Tp, typename Aloc = std::allocator<node<Tp>>>
void copy(node_list<Tp, Aloc> &dst, const node_list<Tp, Aloc> &src)
{
  dst.head_ = src.head_;
  dst.size_ = src.size_;
  dst.allocator = src.allocator;

  node<Tp> *cur_other = src.head_;
  while (cur_other) {
    dst->push_back(cur_other->value);
    cur_other = cur_other->next;
  }
}


/**
 * Discription of the container for working with a single-linked list.
 *
 * @tparam T - the type of variable stored in the node.
 * @tparam A - allocator, memory manager for working with container. Default on
 *             std::allocator.
 */
template<typename T, typename A = std::allocator<node<T>>>
class node_list
{
  public:
    /* Aliases */
    using node_t = node<T>;
    using allocator_t =
            typename std::allocator_traits<A>::template rebind_alloc<node_t>;
    using iterator_t = node_iterator<T>;
    using const_iterator_t = const node_iterator<T>;

    /**
     * The default constructor.
     */
    node_list() = default;

    /**
     * The distructor
     */
    virtual ~node_list() {
      while (head_) {
        node_t *next = head_->next;
        allocator.destroy(&head_->value);
        allocator.deallocate(head_, 1);
        head_ = next;
      }
    }

    /**
     * @brief Copy constructor.
     * @param other [in] - the object to copy.
     */
    node_list(const node_list &other) {
      copy(this, other);
    }

    /**
     * @brief Move constructor.
     * @param other [in] - the object to move.
     */
    node_list(node_list &&other) {
      swap(this, other);
    }

    /**
     * @brief Copy operator.
     * @param other [in] - the object to copy.
     */
    node_list & operator=(const node_list & other) {
      copy(this, other);
      return *this;
    }

    /**
     * @brief Move operator.
     * @param other [in] - the object to move.
     */
    node_list & operator=(node_list &&other) {
      swap(this, other);
      return *this;
    }

    /**
     * @brief  The begin iterator of the node list.
     * @return Returns an iterator to the beginning of the node list.
     */
    iterator_t begin() {
      return iterator_t(head_);
    }

    /**
     * @brief  The end iterator of the node list.
     * @return Returns an iterator to the end of the node list.
     */
    iterator_t end() {
      return iterator_t();
    }

    /**
     * @brief  The const begin iterator of the node list.
     * @return Returns an const iterator to the beginning of the node list.
     */
    const_iterator_t cbegin() const {
      return const_iterator_t(head_);
    }

    /**
     * @brief  The const end iterator of the node list.
     * @return Returns an const iterator to the end of the node list.
     */
    const_iterator_t cend() const { return const_iterator_t(); }

    /**
     * @brief The number of data in the node list.
     * @return The number of data.
     */
    std::size_t size() {
      return size_;
    }

    /**
     * @brief Add an item to the top of the list.
     * @param value [in] - add value.
     */
    void push_front(T &value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, value});
      push_front_helper(new_node);
    }

    /**
     * @brief Add an item to the top of the list.
     * @param value [in] - add value.
     */
    void push_front(T &&value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, std::move(value)});
      push_front_helper(new_node);
    }

    /**
     * @brief Add an item to the top of the list (variable number of params).
     * @tparam ...Args - params.
     * @param args [in] - add value.
     */
    template<typename... Args>
    void push_front(Args &&... args) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, std::forward<Args>(args)...);
      push_front_helper(new_node);
    }

    /**
     * @brief Add an item to the back of the list.
     * @param value [in] - add value.
     */
    void push_back(T &value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, value});
      push_back_helper(new_node);
    }

    /**
     * @brief Add an item to the back of the list.
     * @param value [in] - add value.
     */
    void push_back(T &&value) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, node_t{nullptr, std::move(value)});
      push_back_helper(new_node);
    }

    /**
     * @brief Add an item to the back of the list (variable number of params).
     * @tparam ...Args - params.
     * @param args [in] - add value.
     */
    template<typename... Args>
    void push_back(Args &&... args) {
      node_t *new_node = allocator.allocate(1);
      allocator.construct(new_node, std::forward<Args>(args)...);
      push_back_helper(new_node);
    }


  private:
    std::size_t size_ = 0;    /**< - number of data in the node list */
    node_t *head_ = nullptr;  /**< - pointer to the head on the list */
    allocator_t allocator{};  /**< - memory manager */

    /* Friends function */
    template<typename Tp, typename Aloc>
    friend void swap(node_list<Tp, Aloc> &dst, node_list<Tp, Aloc> &src);

    template<typename Tp, typename Aloc>
    friend void copy(node_list<Tp, Aloc> &dst, const node_list<Tp, Aloc> &src);

    /**
     * @brief The push_back helper function.
     * @param ptr_new_node [in] - pointer to the new node.
     */
    void push_back_helper(node_t * const ptr_new_node) {
      if (head_ == nullptr)
        head_ = ptr_new_node;
      else {
        node_t *next_node = head_->next;
        if (next_node == nullptr)
          head_->next = ptr_new_node;
        else {
          while (next_node->next)
            next_node = next_node->next;

          next_node->next = ptr_new_node;
        }
      }
      ++size_;
    }

    /**
     * @brief The push_front helper function.
     * @param ptr_new_node [in] - pointer to the new node.
     */
    void push_front_helper(node_t *ptr_new_node) {
      ptr_new_node->next = head_;
      head_ = ptr_new_node;
      ++size_;
    }
};

#endif /* NODELIST_H_ */
