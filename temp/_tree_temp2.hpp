#ifndef _TREE_HPP
# define _TREE_HPP

#include "iterator.hpp"

#include <memory>

namespace ft
{
  enum _Rb_tree_color { _S_red = false, _S_black = true };

  struct _Rb_tree_node_base
  {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

    _Rb_tree_color  _M_color;
    _Base_ptr       _M_parent;
    _Base_ptr       _M_left;
    _Base_ptr       _M_right;

    static _Base_ptr
    _S_minimum(_Base_ptr __x) {
      while (__x->_M_left != 0)
        __x = __x->_M_left;
      return (__x);
    }

    static _Const_Base_ptr
    _S_minimum(_Const_Base_ptr __x) {
      while (__x->_M_left != 0)
        __x = __x->_M_left;
      return (__x);
    }

    static _Base_ptr
    _S_maximum(_Base_ptr __x) {
      while (__x->_M_right != 0)
        __x = __x->_M_right;
      return (__x);
    }

    static _Const_Base_ptr
    _S_maximum(_Const_Base_ptr __x) {
      while (__x->_M_right != 0)
        __x = __x->_M_right;
      return (__x);
    }
  };

  template<typename _Val>
  struct _Rb_tree_node : public _Rb_tree_node_base
  {
    typedef _Rb_tree_node<_Val>* _Link_type;

    _Val _M_value_field;
  };

struct _Rb_tree_base_iterator
{
  typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  _Base_ptr _M_node;

  void _M_increment()
  {
    if (_M_node->_M_right != 0) {
      _M_node = _M_node->_M_right;
      while (_M_node->_M_left != 0)
        _M_node = _M_node->_M_left;
    }
    else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_right) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      if (_M_node->_M_right != __y)
        _M_node = __y;
    }
  }

  void _M_decrement()
  {
    if (_M_node->_M_color == _S_red &&
        _M_node->_M_parent->_M_parent == _M_node)
      _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
      _Base_ptr __y = _M_node->_M_left;
      while (__y->_M_right != 0)
        __y = __y->_M_right;
      _M_node = __y;
    }
    else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_left) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      _M_node = __y;
    }
  }
};

template <class _Value, class _Ref, class _Ptr>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator
{
  typedef _Value value_type;
  typedef _Ref reference;
  typedef _Ptr pointer;
  typedef _Rb_tree_iterator<_Value, _Value&, _Value*>             
    iterator;
  typedef _Rb_tree_iterator<_Value, const _Value&, const _Value*> 
    const_iterator;
  typedef _Rb_tree_iterator<_Value, _Ref, _Ptr>                   
    _Self;
  typedef _Rb_tree_node<_Value>* _Link_type;

  _Rb_tree_iterator() {}
  _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
  _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }

  reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
  pointer operator->() const { return &(operator*()); }

  _Self& operator++() { _M_increment(); return *this; }
  _Self operator++(int) {
    _Self __tmp = *this;
    _M_increment();
    return __tmp;
  }
    
  _Self& operator--() { _M_decrement(); return *this; }
  _Self operator--(int) {
    _Self __tmp = *this;
    _M_decrement();
    return __tmp;
  }
         iterator
       _M_const_cast() const
       { return iterator(static_cast<typename iterator::_Link_type>
             (const_cast<typename iterator::_Base_ptr>(_M_node))); }
};
template <class _Value, class _Ref, class _Ptr>
inline bool operator==(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
		       const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) {
  return __x._M_node == __y._M_node;
}

template <class _Value>
inline bool operator==(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
		       const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
  return __x._M_node == __y._M_node;
}

template <class _Value>
inline bool operator==(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
		       const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
  return __x._M_node == __y._M_node;
}

template <class _Value, class _Ref, class _Ptr>
inline bool operator!=(const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __x,
		       const _Rb_tree_iterator<_Value, _Ref, _Ptr>& __y) {
  return __x._M_node != __y._M_node;
}

template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __x,
		       const _Rb_tree_iterator<_Value, _Value&, _Value*>& __y) {
  return __x._M_node != __y._M_node;
}

template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value, _Value&, _Value*>& __x,
		       const _Rb_tree_iterator<_Value, const _Value&, const _Value*>& __y) {
  return __x._M_node != __y._M_node;
}

inline void 
_Rb_tree_rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  if (__y->_M_left !=0)
    __y->_M_left->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else
    __x->_M_parent->_M_right = __y;
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

inline void 
_Rb_tree_rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  if (__y->_M_right != 0)
    __y->_M_right->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;
  __y->_M_right = __x;
  __x->_M_parent = __y;
}

inline void 
_Rb_tree_rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  __x->_M_color = _S_red;
  while (__x != __root && __x->_M_parent->_M_color == _S_red) {
    if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
      if (__y && __y->_M_color == _S_red) {
        __x->_M_parent->_M_color = _S_black;
        __y->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;
          _Rb_tree_rotate_left(__x, __root);
        }
        __x->_M_parent->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        _Rb_tree_rotate_right(__x->_M_parent->_M_parent, __root);
      }
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
      if (__y && __y->_M_color == _S_red) {
        __x->_M_parent->_M_color = _S_black;
        __y->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _Rb_tree_rotate_right(__x, __root);
        }
        __x->_M_parent->_M_color = _S_black;
        __x->_M_parent->_M_parent->_M_color = _S_red;
        _Rb_tree_rotate_left(__x->_M_parent->_M_parent, __root);
      }
    }
  }
  __root->_M_color = _S_black;
}

inline _Rb_tree_node_base*
_Rb_tree_rebalance_for_erase(_Rb_tree_node_base* __z,
                             _Rb_tree_node_base*& __root,
                             _Rb_tree_node_base*& __leftmost,
                             _Rb_tree_node_base*& __rightmost)
{
  _Rb_tree_node_base* __y = __z;
  _Rb_tree_node_base* __x = 0;
  _Rb_tree_node_base* __x_parent = 0;
  if (__y->_M_left == 0)     // __z has at most one non-null child. y == z.
    __x = __y->_M_right;     // __x might be null.
  else
    if (__y->_M_right == 0)  // __z has exactly one non-null child. y == z.
      __x = __y->_M_left;    // __x is not null.
    else {                   // __z has two non-null children.  Set __y to
      __y = __y->_M_right;   //   __z's successor.  __x might be null.
      while (__y->_M_left != 0)
        __y = __y->_M_left;
      __x = __y->_M_right;
    }
  if (__y != __z) {          // relink y in place of z.  y is z's successor
    __z->_M_left->_M_parent = __y; 
    __y->_M_left = __z->_M_left;
    if (__y != __z->_M_right) {
      __x_parent = __y->_M_parent;
      if (__x) __x->_M_parent = __y->_M_parent;
      __y->_M_parent->_M_left = __x;      // __y must be a child of _M_left
      __y->_M_right = __z->_M_right;
      __z->_M_right->_M_parent = __y;
    }
    else
      __x_parent = __y;  
    if (__root == __z)
      __root = __y;
    else if (__z->_M_parent->_M_left == __z)
      __z->_M_parent->_M_left = __y;
    else 
      __z->_M_parent->_M_right = __y;
    __y->_M_parent = __z->_M_parent;
    std::swap(__y->_M_color, __z->_M_color);
    __y = __z;
    // __y now points to node to be actually deleted
  }
  else {                        // __y == __z
    __x_parent = __y->_M_parent;
    if (__x) __x->_M_parent = __y->_M_parent;   
    if (__root == __z)
      __root = __x;
    else 
      if (__z->_M_parent->_M_left == __z)
        __z->_M_parent->_M_left = __x;
      else
        __z->_M_parent->_M_right = __x;
    if (__leftmost == __z) 
      if (__z->_M_right == 0)        // __z->_M_left must be null also
        __leftmost = __z->_M_parent;
    // makes __leftmost == _M_header if __z == __root
      else
        __leftmost = _Rb_tree_node_base::_S_minimum(__x);
    if (__rightmost == __z)  
      if (__z->_M_left == 0)         // __z->_M_right must be null also
        __rightmost = __z->_M_parent;  
    // makes __rightmost == _M_header if __z == __root
      else                      // __x == __z->_M_left
        __rightmost = _Rb_tree_node_base::_S_maximum(__x);
  }
  if (__y->_M_color != _S_red) { 
    while (__x != __root && (__x == 0 || __x->_M_color == _S_black))
      if (__x == __x_parent->_M_left) {
        _Rb_tree_node_base* __w = __x_parent->_M_right;
        if (__w->_M_color == _S_red) {
          __w->_M_color = _S_black;
          __x_parent->_M_color = _S_red;
          _Rb_tree_rotate_left(__x_parent, __root);
          __w = __x_parent->_M_right;
        }
        if ((__w->_M_left == 0 || 
             __w->_M_left->_M_color == _S_black) &&
            (__w->_M_right == 0 || 
             __w->_M_right->_M_color == _S_black)) {
          __w->_M_color = _S_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_right == 0 || 
              __w->_M_right->_M_color == _S_black) {
            if (__w->_M_left) __w->_M_left->_M_color = _S_black;
            __w->_M_color = _S_red;
            _Rb_tree_rotate_right(__w, __root);
            __w = __x_parent->_M_right;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_black;
          if (__w->_M_right) __w->_M_right->_M_color = _S_black;
          _Rb_tree_rotate_left(__x_parent, __root);
          break;
        }
      } else {                  // same as above, with _M_right <-> _M_left.
        _Rb_tree_node_base* __w = __x_parent->_M_left;
        if (__w->_M_color == _S_red) {
          __w->_M_color = _S_black;
          __x_parent->_M_color = _S_red;
          _Rb_tree_rotate_right(__x_parent, __root);
          __w = __x_parent->_M_left;
        }
        if ((__w->_M_right == 0 || 
             __w->_M_right->_M_color == _S_black) &&
            (__w->_M_left == 0 || 
             __w->_M_left->_M_color == _S_black)) {
          __w->_M_color = _S_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_left == 0 || 
              __w->_M_left->_M_color == _S_black) {
            if (__w->_M_right) __w->_M_right->_M_color = _S_black;
            __w->_M_color = _S_red;
            _Rb_tree_rotate_left(__w, __root);
            __w = __x_parent->_M_left;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _S_black;
          if (__w->_M_left) __w->_M_left->_M_color = _S_black;
          _Rb_tree_rotate_right(__x_parent, __root);
          break;
        }
      }
    if (__x) __x->_M_color = _S_black;
  }
  return __y;
}


//   _Rb_tree_node_base*
//   _Rb_tree_increment(_Rb_tree_node_base* __x) throw ();
//    const _Rb_tree_node_base*
//   _Rb_tree_increment(const _Rb_tree_node_base* __x) throw ();
//    _Rb_tree_node_base*
//   _Rb_tree_decrement(_Rb_tree_node_base* __x) throw ();
//    const _Rb_tree_node_base*
//   _Rb_tree_decrement(const _Rb_tree_node_base* __x) throw ();

// template<typename _Tp>
//   struct _Rb_tree_iterator
//   {
//     typedef _Tp  value_type;
//     typedef _Tp& reference;
//     typedef _Tp* pointer;

//     typedef bidirectional_iterator_tag iterator_category;
//     typedef ptrdiff_t                  difference_type;

//     typedef _Rb_tree_iterator<_Tp>        _Self;
//     typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
//     typedef _Rb_tree_node<_Tp>*           _Link_type;

//     _Rb_tree_iterator()
//     : _M_node() { }

//     explicit
//     _Rb_tree_iterator(_Link_type __x)
//     : _M_node(__x) { }


//   void _M_increment()
//   {
//     if (_M_node->_M_right != 0) {
//       _M_node = _M_node->_M_right;
//       while (_M_node->_M_left != 0)
//         _M_node = _M_node->_M_left;
//     }
//     else {
//       _Base_ptr __y = _M_node->_M_parent;
//       while (_M_node == __y->_M_right) {
//         _M_node = __y;
//         __y = __y->_M_parent;
//       }
//       if (_M_node->_M_right != __y)
//         _M_node = __y;
//     }
//   }

//   void _M_decrement()
//   {
//     if (_M_node->_M_color == _S_red &&
//         _M_node->_M_parent->_M_parent == _M_node)
//       _M_node = _M_node->_M_right;
//     else if (_M_node->_M_left != 0) {
//       _Base_ptr __y = _M_node->_M_left;
//       while (__y->_M_right != 0)
//         __y = __y->_M_right;
//       _M_node = __y;
//     }
//     else {
//       _Base_ptr __y = _M_node->_M_parent;
//       while (_M_node == __y->_M_left) {
//         _M_node = __y;
//         __y = __y->_M_parent;
//       }
//       _M_node = __y;
//     }
//   }

//     reference
//     operator*() const
//     { return static_cast<_Link_type>(_M_node)->_M_value_field; }

//     pointer
//     operator->() const
//     { return addressof(static_cast<_Link_type>
//               (_M_node)->_M_value_field); }

//     _Self&
//     operator++()
//     {
//   _M_node = _M_increment(_M_node);
//   return *this;
//     }

//     _Self
//     operator++(int)
//     {
//   _Self __tmp = *this;
//   _M_node = _M_increment(_M_node);
//   return __tmp;
//     }

//     _Self&
//     operator--()
//     {
//   _M_node = _M_decrement(_M_node);
//   return *this;
//     }

//     _Self
//     operator--(int)
//     {
//   _Self __tmp = *this;
//   _M_node = _M_decrement(_M_node);
//   return __tmp;
//     }

//     bool
//     operator==(const _Self& __x) const
//     { return _M_node == __x._M_node; }

//     bool
//     operator!=(const _Self& __x) const
//     { return _M_node != __x._M_node; }

//     _Base_ptr _M_node;
// };

// template<typename _Tp>
//   struct _Rb_tree_const_iterator
//   {
//     typedef _Tp        value_type;
//     typedef const _Tp& reference;
//     typedef const _Tp* pointer;

//     typedef _Rb_tree_iterator<_Tp> iterator;

//     typedef bidirectional_iterator_tag iterator_category;
//     typedef ptrdiff_t                  difference_type;

//     typedef _Rb_tree_const_iterator<_Tp>        _Self;
//     typedef _Rb_tree_node_base::_Const_Base_ptr _Base_ptr;
//     typedef const _Rb_tree_node<_Tp>*           _Link_type;

//     _Rb_tree_const_iterator()
//     : _M_node() { }

//     explicit
//     _Rb_tree_const_iterator(_Link_type __x)
//     : _M_node(__x) { }

//     _Rb_tree_const_iterator(const iterator& __it)
//     : _M_node(__it._M_node) { }

//     iterator
//     _M_const_cast() const
//     { return iterator(static_cast<typename iterator::_Link_type>
//           (const_cast<typename iterator::_Base_ptr>(_M_node))); }

//     reference
//     operator*() const
//     { return static_cast<_Link_type>(_M_node)->_M_value_field; }

//     pointer
//     operator->() const
//     { return addressof(static_cast<_Link_type>
//               (_M_node)->_M_value_field); }

//     _Self&
//     operator++()
//     {
//   _M_node = _Rb_tree_increment(_M_node);
//   return *this;
//     }

//     _Self
//     operator++(int)
//     {
//   _Self __tmp = *this;
//   _M_node = _Rb_tree_increment(_M_node);
//   return __tmp;
//     }

//     _Self&
//     operator--()
//     {
//   _M_node = _M_decrement(_M_node);
//   return *this;
//     }

//     _Self
//     operator--(int)
//     {
//   _Self __tmp = *this;
//   _M_node = _M_decrement(_M_node);
//   return __tmp;
//     }

//     bool
//     operator==(const _Self& __x) const
//     { return _M_node == __x._M_node; }

//     bool
//     operator!=(const _Self& __x) const
//     { return _M_node != __x._M_node; }

//     _Base_ptr _M_node;
//   };

// template<typename _Val>
//   inline bool
//   operator==(const _Rb_tree_iterator<_Val>& __x,
//             const _Rb_tree_const_iterator<_Val>& __y)
//   { return __x._M_node == __y._M_node; }

// template<typename _Val>
//   inline bool
//   operator!=(const _Rb_tree_iterator<_Val>& __x,
//             const _Rb_tree_const_iterator<_Val>& __y)
//   { return __x._M_node != __y._M_node; }

void
_Rb_tree_insert_and_rebalance(const bool __insert_left,
                              _Rb_tree_node_base* __x,
                              _Rb_tree_node_base* __p,
                              _Rb_tree_node_base& __header) throw ()
{

}

template<typename _Key, typename _Val, typename _KeyOfValue,
        typename _Compare, typename _Alloc = std::allocator<_Val> >
  class _Rb_tree
  {
    typedef typename _Alloc::template rebind<_Rb_tree_node<_Val> >::other
            _Node_allocator;

  protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef const _Rb_tree_node_base* _Const_Base_ptr;

  public:
    typedef _Key key_type;
    typedef _Val value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef _Rb_tree_node<_Val>* _Link_type;
    typedef const _Rb_tree_node<_Val>* _Const_Link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Alloc allocator_type;

    _Node_allocator&
    _M_get_Node_allocator()
    { return *static_cast<_Node_allocator*>(&this->_M_impl); }
    
    const _Node_allocator&
    _M_get_Node_allocator() const
    { return *static_cast<const _Node_allocator*>(&this->_M_impl); }

    allocator_type
    get_allocator() const
    { return allocator_type(_M_get_Node_allocator()); }

  protected:
    _Link_type
    _M_get_node()
    { return _M_impl._Node_allocator::allocate(1); }

    void
    _M_put_node(_Link_type __p)
    { _M_impl._Node_allocator::deallocate(__p, 1); }

      _Link_type
      _M_create_node(const value_type& __x)
      {
    _Link_type __tmp = _M_get_node();
    try
      { get_allocator().construct
          (std::addressof(__tmp->_M_value_field), __x); }
    catch(...)
      {
        _M_put_node(__tmp);
        throw ;
      }
    return __tmp;
      }

      void
      _M_destroy_node(_Link_type __p)
      {
    get_allocator().destroy(std::addressof(__p->_M_value_field));
    _M_put_node(__p);
      }

    _Link_type
    _M_clone_node(_Const_Link_type __x)
    {
  _Link_type __tmp = _M_create_node(__x->_M_value_field);
  __tmp->_M_color = __x->_M_color;
  __tmp->_M_left = 0;
  __tmp->_M_right = 0;
  return __tmp;
    }

  protected:
    template<typename _Key_compare, 
        bool _Is_pod_comparator = __is_pod(_Key_compare)>
    struct _Rb_tree_impl : public _Node_allocator
    {
    _Key_compare      _M_key_compare;
    _Rb_tree_node_base    _M_header;
    size_type         _M_node_count; // Keeps track of size of tree.

    _Rb_tree_impl()
    : _Node_allocator(), _M_key_compare(), _M_header(),
      _M_node_count(0)
    { _M_initialize(); }

    _Rb_tree_impl(const _Key_compare& __comp, const _Node_allocator& __a)
    : _Node_allocator(__a), _M_key_compare(__comp), _M_header(),
      _M_node_count(0)
    { _M_initialize(); }

  private:
    void
    _M_initialize()
    {
      this->_M_header._M_color = _S_red;
      this->_M_header._M_parent = 0;
      this->_M_header._M_left = &this->_M_header;
      this->_M_header._M_right = &this->_M_header;
    }     
    };
    _Rb_tree_impl<_Compare> _M_impl;

    protected:
      _Base_ptr&
      _M_root()
      { return this->_M_impl._M_header._M_parent; }

      _Const_Base_ptr
      _M_root() const
      { return this->_M_impl._M_header._M_parent; }

      _Base_ptr&
      _M_leftmost()
      { return this->_M_impl._M_header._M_left; }

      _Const_Base_ptr
      _M_leftmost() const
      { return this->_M_impl._M_header._M_left; }

      _Base_ptr&
      _M_rightmost()
      { return this->_M_impl._M_header._M_right; }

      _Const_Base_ptr
      _M_rightmost() const
      { return this->_M_impl._M_header._M_right; }

      _Link_type
      _M_begin()
      { return static_cast<_Link_type>(this->_M_impl._M_header._M_parent); }

      _Const_Link_type
      _M_begin() const
      {
    return static_cast<_Const_Link_type>
      (this->_M_impl._M_header._M_parent);
      }

      _Link_type
      _M_end()
      { return static_cast<_Link_type>(&this->_M_impl._M_header); }

      _Const_Link_type
      _M_end() const
      { return static_cast<_Const_Link_type>(&this->_M_impl._M_header); }

      static const_reference
      _S_value(_Const_Link_type __x)
      { return __x->_M_value_field; }

      static const _Key&
      _S_key(_Const_Link_type __x)
      { return _KeyOfValue()(_S_value(__x)); }

      static _Link_type
      _S_left(_Base_ptr __x)
      { return static_cast<_Link_type>(__x->_M_left); }

      static _Const_Link_type
      _S_left(_Const_Base_ptr __x)
      { return static_cast<_Const_Link_type>(__x->_M_left); }

      static _Link_type
      _S_right(_Base_ptr __x)
      { return static_cast<_Link_type>(__x->_M_right); }

      static _Const_Link_type
      _S_right(_Const_Base_ptr __x)
      { return static_cast<_Const_Link_type>(__x->_M_right); }

      static const_reference
      _S_value(_Const_Base_ptr __x)
      { return static_cast<_Const_Link_type>(__x)->_M_value_field; }

      static const _Key&
      _S_key(_Const_Base_ptr __x)
      { return _KeyOfValue()(_S_value(__x)); }

      static _Base_ptr
      _S_minimum(_Base_ptr __x)
      { return _Rb_tree_node_base::_S_minimum(__x); }

      static _Const_Base_ptr
      _S_minimum(_Const_Base_ptr __x)
      { return _Rb_tree_node_base::_S_minimum(__x); }

      static _Base_ptr
      _S_maximum(_Base_ptr __x)
      { return _Rb_tree_node_base::_S_maximum(__x); }

      static _Const_Base_ptr
      _S_maximum(_Const_Base_ptr __x)
      { return _Rb_tree_node_base::_S_maximum(__x); }

    public:
    typedef _Rb_tree_iterator<value_type, reference, pointer> iterator;
    typedef _Rb_tree_iterator<value_type, const_reference, const_pointer> 
            const_iterator;
      typedef std::reverse_iterator<iterator>       reverse_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
      iterator
      _M_insert_(_Const_Base_ptr __x, _Const_Base_ptr __y,
        const value_type& __v);

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 233. Insertion hints in associative containers.
      iterator
      _M_insert_lower(_Base_ptr __x, _Base_ptr __y, const value_type& __v);

      iterator
      _M_insert_equal_lower(const value_type& __x);

      _Link_type
      _M_copy(_Const_Link_type __x, _Link_type __p);

      void
      _M_erase(_Link_type __x);

      iterator
      _M_lower_bound(_Link_type __x, _Link_type __y,
            const _Key& __k);

      const_iterator
      _M_lower_bound(_Const_Link_type __x, _Const_Link_type __y,
            const _Key& __k) const;

      iterator
      _M_upper_bound(_Link_type __x, _Link_type __y,
            const _Key& __k);

      const_iterator
      _M_upper_bound(_Const_Link_type __x, _Const_Link_type __y,
            const _Key& __k) const;

    public:
      // allocation/deallocation
      _Rb_tree() { }

      _Rb_tree(const _Compare& __comp,
          const allocator_type& __a = allocator_type())
      : _M_impl(__comp, __a) { }

      _Rb_tree(const _Rb_tree& __x)
      : _M_impl(__x._M_impl._M_key_compare, __x._M_get_Node_allocator())
      {
    if (__x._M_root() != 0)
      {
        _M_root() = _M_copy(__x._M_begin(), _M_end());
        _M_leftmost() = _S_minimum(_M_root());
        _M_rightmost() = _S_maximum(_M_root());
        _M_impl._M_node_count = __x._M_impl._M_node_count;
      }
      }

      ~_Rb_tree()
      { _M_erase(_M_begin()); }

      _Rb_tree&
      operator=(const _Rb_tree& __x);

      // Accessors.
      _Compare
      key_comp() const
      { return _M_impl._M_key_compare; }

      iterator
      begin()
      { 
    return iterator(static_cast<_Link_type>
            (this->_M_impl._M_header._M_left));
      }

      const_iterator
      begin() const
      { 
    return const_iterator(static_cast<_Const_Link_type>
                  (this->_M_impl._M_header._M_left));
      }

      iterator
      end()
      { return iterator(static_cast<_Link_type>(&this->_M_impl._M_header)); }

      const_iterator
      end() const
      { 
    return const_iterator(static_cast<_Const_Link_type>
                  (&this->_M_impl._M_header));
      }

      reverse_iterator
      rbegin()
      { return reverse_iterator(end()); }

      const_reverse_iterator
      rbegin() const
      { return const_reverse_iterator(end()); }

      reverse_iterator
      rend()
      { return reverse_iterator(begin()); }

      const_reverse_iterator
      rend() const
      { return const_reverse_iterator(begin()); }

      bool
      empty() const
      { return _M_impl._M_node_count == 0; }

      size_type
      size() const
      { return _M_impl._M_node_count; }

      size_type
      max_size() const
      { return _M_get_Node_allocator().max_size(); }

      void
      swap(_Rb_tree& __t);      

      // Insert/erase.
      pair<iterator, bool>
      _M_insert_unique(const value_type& __x);

      iterator
      _M_insert_equal(const value_type& __x);

      iterator
      _M_insert_unique_(const_iterator __position, const value_type& __x);

      iterator

      _M_insert_equal_(const_iterator __position, const value_type& __x);

      template<typename _InputIterator>
        void
        _M_insert_unique(_InputIterator __first, _InputIterator __last);

      template<typename _InputIterator>
        void
        _M_insert_equal(_InputIterator __first, _InputIterator __last);

    private:
      void
      _M_erase_aux(const_iterator __position);

      void
      _M_erase_aux(const_iterator __first, const_iterator __last);

    public:

      void
      erase(iterator __position)
      { _M_erase_aux(__position); }

      void
      erase(const_iterator __position)
      { _M_erase_aux(__position); }
      size_type
      erase(const key_type& __x);
      void
      erase(iterator __first, iterator __last)
      { _M_erase_aux(__first, __last); }

      void
      erase(const_iterator __first, const_iterator __last)
      { _M_erase_aux(__first, __last); }
      void
      erase(const key_type* __first, const key_type* __last);

      void
      clear()
      {
        _M_erase(_M_begin());
        _M_leftmost() = _M_end();
        _M_root() = 0;
        _M_rightmost() = _M_end();
        _M_impl._M_node_count = 0;
      }

      // Set operations.
      iterator
      find(const key_type& __k);

      const_iterator
      find(const key_type& __k) const;

      size_type
      count(const key_type& __k) const;

      iterator
      lower_bound(const key_type& __k)
      { return _M_lower_bound(_M_begin(), _M_end(), __k); }

      const_iterator
      lower_bound(const key_type& __k) const
      { return _M_lower_bound(_M_begin(), _M_end(), __k); }

      iterator
      upper_bound(const key_type& __k)
      { return _M_upper_bound(_M_begin(), _M_end(), __k); }

      const_iterator
      upper_bound(const key_type& __k) const
      { return _M_upper_bound(_M_begin(), _M_end(), __k); }

      pair<iterator, iterator>
      equal_range(const key_type& __k);

      pair<const_iterator, const_iterator>
      equal_range(const key_type& __k) const;

      // Debugging.
      bool
      __rb_verify() const;
    };

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    inline bool
    operator==(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
    {
      return __x.size() == __y.size()
        && ft::equal(__x.begin(), __x.end(), __y.begin());
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    inline bool
    operator<(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
    {
      return ft::lexicographical_compare(__x.begin(), __x.end(), 
                      __y.begin(), __y.end());
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    inline bool
    operator!=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
    { return !(__x == __y); }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    inline bool
    operator>(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
    { return __y < __x; }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    inline bool
    operator<=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
    { return !(__y < __x); }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    inline bool
    operator>=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
          const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
    { return !(__x < __y); }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    inline void
    swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y)
    { __x.swap(__y); }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>&
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    operator=(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x)
    {
      if (this != &__x)
    {
      // Note that _Key may be a constant type.
      clear();
      _M_impl._M_key_compare = __x._M_impl._M_key_compare;
      if (__x._M_root() != 0)
        {
          _M_root() = _M_copy(__x._M_begin(), _M_end());
          _M_leftmost() = _S_minimum(_M_root());
          _M_rightmost() = _S_maximum(_M_root());
          _M_impl._M_node_count = __x._M_impl._M_node_count;
        }
    }
      return *this;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_insert_(_Const_Base_ptr __x, _Const_Base_ptr __p, const _Val& __v)
    {
      bool __insert_left = (__x != 0 || __p == _M_end()
                || _M_impl._M_key_compare(_KeyOfValue()(__v), 
                              _S_key(__p)));

      _Link_type __z = _M_create_node(__v);

      _Rb_tree_insert_and_rebalance(__insert_left, __z,
                    const_cast<_Base_ptr>(__p),  
                    this->_M_impl._M_header);
      ++_M_impl._M_node_count;
      return iterator(__z);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_insert_lower(_Base_ptr __x, _Base_ptr __p, const _Val& __v)
    {
      bool __insert_left = (__x != 0 || __p == _M_end()
                || !_M_impl._M_key_compare(_S_key(__p),
                              _KeyOfValue()(__v)));

      _Link_type __z = _M_create_node(__v);

      _Rb_tree_insert_and_rebalance(__insert_left, __z, __p,  
                    this->_M_impl._M_header);
      ++_M_impl._M_node_count;
      return iterator(__z);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_insert_equal_lower(const _Val& __v)
    {
      _Link_type __x = _M_begin();
      _Link_type __y = _M_end();
      while (__x != 0)
    {
      __y = __x;
      __x = !_M_impl._M_key_compare(_S_key(__x), _KeyOfValue()(__v)) ?
            _S_left(__x) : _S_right(__x);
    }
      return _M_insert_lower(__x, __y, __v);
    }

  template<typename _Key, typename _Val, typename _KoV,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_Link_type
    _Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::
    _M_copy(_Const_Link_type __x, _Link_type __p)
    {
      // Structural copy.  __x and __p must be non-null.
      _Link_type __top = _M_clone_node(__x);
      __top->_M_parent = __p;

    try
    {
      if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
      __p = __top;
      __x = _S_left(__x);

      while (__x != 0)
        {
          _Link_type __y = _M_clone_node(__x);
          __p->_M_left = __y;
          __y->_M_parent = __p;
          if (__x->_M_right)
        __y->_M_right = _M_copy(_S_right(__x), __y);
          __p = __y;
          __x = _S_left(__x);
        }
    }
    catch(...)
    {
      _M_erase(__top);
      throw ;
    }
      return __top;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    void
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_erase(_Link_type __x)
    {
      // Erase without rebalancing.
      while (__x != 0)
    {
      _M_erase(_S_right(__x));
      _Link_type __y = _S_left(__x);
      _M_destroy_node(__x);
      __x = __y;
    }
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_lower_bound(_Link_type __x, _Link_type __y,
          const _Key& __k)
    {
      while (__x != 0)
    if (!_M_impl._M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
      return iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_lower_bound(_Const_Link_type __x, _Const_Link_type __y,
          const _Key& __k) const
    {
      while (__x != 0)
    if (!_M_impl._M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
      return const_iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_upper_bound(_Link_type __x, _Link_type __y,
          const _Key& __k)
    {
      while (__x != 0)
    if (_M_impl._M_key_compare(__k, _S_key(__x)))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
      return iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_upper_bound(_Const_Link_type __x, _Const_Link_type __y,
          const _Key& __k) const
    {
      while (__x != 0)
    if (_M_impl._M_key_compare(__k, _S_key(__x)))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
      return const_iterator(__y);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::iterator,
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::iterator>
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    equal_range(const _Key& __k)
    {
      _Link_type __x = _M_begin();
      _Link_type __y = _M_end();
      while (__x != 0)
    {
      if (_M_impl._M_key_compare(_S_key(__x), __k))
        __x = _S_right(__x);
      else if (_M_impl._M_key_compare(__k, _S_key(__x)))
        __y = __x, __x = _S_left(__x);
      else
        {
          _Link_type __xu(__x), __yu(__y);
          __y = __x, __x = _S_left(__x);
          __xu = _S_right(__xu);
          return pair<iterator,
                  iterator>(_M_lower_bound(__x, __y, __k),
                    _M_upper_bound(__xu, __yu, __k));
        }
    }
    return pair<iterator, iterator>(iterator(__y),
                    iterator(__y));
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::const_iterator,
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::const_iterator>
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    equal_range(const _Key& __k) const
    {
      _Const_Link_type __x = _M_begin();
      _Const_Link_type __y = _M_end();
      while (__x != 0)
    {
      if (_M_impl._M_key_compare(_S_key(__x), __k))
        __x = _S_right(__x);
      else if (_M_impl._M_key_compare(__k, _S_key(__x)))
        __y = __x, __x = _S_left(__x);
      else
        {
          _Const_Link_type __xu(__x), __yu(__y);
          __y = __x, __x = _S_left(__x);
          __xu = _S_right(__xu);
          return pair<const_iterator,
                  const_iterator>(_M_lower_bound(__x, __y, __k),
                      _M_upper_bound(__xu, __yu, __k));
        }
    }
      return pair<const_iterator, const_iterator>(const_iterator(__y),
                          const_iterator(__y));
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    void
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __t)
    {
      if (_M_root() == 0)
    {
      if (__t._M_root() != 0)
        {
          _M_root() = __t._M_root();
          _M_leftmost() = __t._M_leftmost();
          _M_rightmost() = __t._M_rightmost();
          _M_root()->_M_parent = _M_end();
          
          __t._M_root() = 0;
          __t._M_leftmost() = __t._M_end();
          __t._M_rightmost() = __t._M_end();
        }
    }
      else if (__t._M_root() == 0)
    {
      __t._M_root() = _M_root();
      __t._M_leftmost() = _M_leftmost();
      __t._M_rightmost() = _M_rightmost();
      __t._M_root()->_M_parent = __t._M_end();
      
      _M_root() = 0;
      _M_leftmost() = _M_end();
      _M_rightmost() = _M_end();
    }
      else
    {
      std::swap(_M_root(),__t._M_root());
      std::swap(_M_leftmost(),__t._M_leftmost());
      std::swap(_M_rightmost(),__t._M_rightmost());
      
      _M_root()->_M_parent = _M_end();
      __t._M_root()->_M_parent = __t._M_end();
    }
      // No need to swap header's color as it does not change.
      std::swap(this->_M_impl._M_node_count, __t._M_impl._M_node_count);
      std::swap(this->_M_impl._M_key_compare, __t._M_impl._M_key_compare);
      
    //   // _GLIBCXX_RESOLVE_LIB_DEFECTS
    //   // 431. Swapping containers with unequal allocators.
    //   std::__alloc_swap<_Node_allocator>::
    // _S_do_it(_M_get_Node_allocator(), __t._M_get_Node_allocator());
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::iterator, bool>
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_insert_unique(const _Val& __v)
    {
      _Link_type __x = _M_begin();
      _Link_type __y = _M_end();
      bool __comp = true;
      while (__x != 0)
    {
      __y = __x;
      __comp = _M_impl._M_key_compare(_KeyOfValue()(__v), _S_key(__x));
      __x = __comp ? _S_left(__x) : _S_right(__x);
    }
      iterator __j = iterator(__y);
      if (__comp)
    {
      if (__j == begin())
        return pair<iterator, bool>
          (_M_insert_(__x, __y, __v), true);
      else
        --__j;
    }
      if (_M_impl._M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
    return pair<iterator, bool>
      (_M_insert_(__x, __y, __v), true);
      return pair<iterator, bool>(__j, false);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_insert_equal(const _Val& __v)
    {
      _Link_type __x = _M_begin();
      _Link_type __y = _M_end();
      while (__x != 0)
    {
      __y = __x;
      __x = _M_impl._M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ?
            _S_left(__x) : _S_right(__x);
    }
      return _M_insert_(__x, __y, __v);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_insert_unique_(const_iterator __position, const _Val& __v)
    {
      // end()
      if (__position._M_node == _M_end())
    {
      if (size() > 0
          && _M_impl._M_key_compare(_S_key(_M_rightmost()), 
                    _KeyOfValue()(__v)))
        return _M_insert_(0, _M_rightmost(), __v);
      else
        return _M_insert_unique(__v).first;
    }
      else if (_M_impl._M_key_compare(_KeyOfValue()(__v),
                      _S_key(__position._M_node)))
    {
      // First, try before...
      const_iterator __before = __position;
      if (__position._M_node == _M_leftmost()) // begin()
        return _M_insert_(_M_leftmost(), _M_leftmost(),
                  __v);
      else if (_M_impl._M_key_compare(_S_key((--__before)._M_node), 
                      _KeyOfValue()(__v)))
        {
          if (_S_right(__before._M_node) == 0)
        return _M_insert_(0, __before._M_node,
                  __v);
          else
        return _M_insert_(__position._M_node,
                  __position._M_node,
                  __v);
        }
      else
        return _M_insert_unique(__v).first;
    }
      else if (_M_impl._M_key_compare(_S_key(__position._M_node),
                      _KeyOfValue()(__v)))
    {
      // ... then try after.
      const_iterator __after = __position;
      if (__position._M_node == _M_rightmost())
        return _M_insert_(0, _M_rightmost(),
                  __v);
      else if (_M_impl._M_key_compare(_KeyOfValue()(__v),
                      _S_key((++__after)._M_node)))
        {
          if (_S_right(__position._M_node) == 0)
        return _M_insert_(0, __position._M_node,
                  __v);
          else
        return _M_insert_(__after._M_node, __after._M_node,
                  __v);
        }
      else
        return _M_insert_unique(__v).first;
    }
      else
    // Equivalent keys.
    return __position._M_const_cast();
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_insert_equal_(const_iterator __position, const _Val& __v)
    {
      // end()
      if (__position._M_node == _M_end())
    {
      if (size() > 0
          && !_M_impl._M_key_compare(_KeyOfValue()(__v),
                    _S_key(_M_rightmost())))
        return _M_insert_(0, _M_rightmost(),
                  __v);
      else
        return _M_insert_equal(__v);
    }
      else if (!_M_impl._M_key_compare(_S_key(__position._M_node),
                      _KeyOfValue()(__v)))
    {
      // First, try before...
      const_iterator __before = __position;
      if (__position._M_node == _M_leftmost()) // begin()
        return _M_insert_(_M_leftmost(), _M_leftmost(),
                  __v);
      else if (!_M_impl._M_key_compare(_KeyOfValue()(__v),
                      _S_key((--__before)._M_node)))
        {
          if (_S_right(__before._M_node) == 0)
        return _M_insert_(0, __before._M_node,
                  __v);
          else
        return _M_insert_(__position._M_node,
                  __position._M_node,
                  __v);
        }
      else
        return _M_insert_equal(__v);
    }
      else
    {
      // ... then try after.  
      const_iterator __after = __position;
      if (__position._M_node == _M_rightmost())
        return _M_insert_(0, _M_rightmost(),
                  __v);
      else if (!_M_impl._M_key_compare(_S_key((++__after)._M_node),
                      _KeyOfValue()(__v)))
        {
          if (_S_right(__position._M_node) == 0)
        return _M_insert_(0, __position._M_node,
                  __v);
          else
        return _M_insert_(__after._M_node, __after._M_node,
                  __v);
        }
      else
        return _M_insert_equal_lower(__v);
    }
    }

  template<typename _Key, typename _Val, typename _KoV,
          typename _Cmp, typename _Alloc>
    template<class _II>
      void
      _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::
      _M_insert_unique(_II __first, _II __last)
      {
    for (; __first != __last; ++__first)
      _M_insert_unique_(end(), *__first);
      }

  template<typename _Key, typename _Val, typename _KoV,
          typename _Cmp, typename _Alloc>
    template<class _II>
      void
      _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::
      _M_insert_equal(_II __first, _II __last)
      {
    for (; __first != __last; ++__first)
      _M_insert_equal_(end(), *__first);
      }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    void
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_erase_aux(const_iterator __position)
    {
      _Link_type __y =
    static_cast<_Link_type>(_Rb_tree_rebalance_for_erase
                (const_cast<_Base_ptr>(__position._M_node),
                this->_M_impl._M_header));
      _M_destroy_node(__y);
      --_M_impl._M_node_count;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    void
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    _M_erase_aux(const_iterator __first, const_iterator __last)
    {
      if (__first == begin() && __last == end())
    clear();
      else
    while (__first != __last)
      erase(__first++);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    erase(const _Key& __x)
    {
      pair<iterator, iterator> __p = equal_range(__x);
      const size_type __old_size = size();
      erase(__p.first, __p.second);
      return __old_size - size();
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    void
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    erase(const _Key* __first, const _Key* __last)
    {
      while (__first != __last)
    erase(*__first++);
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    find(const _Key& __k)
    {
      iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
      return (__j == end()
          || _M_impl._M_key_compare(__k,
                    _S_key(__j._M_node))) ? end() : __j;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue,
              _Compare, _Alloc>::const_iterator
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    find(const _Key& __k) const
    {
      const_iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
      return (__j == end()
          || _M_impl._M_key_compare(__k, 
                    _S_key(__j._M_node))) ? end() : __j;
    }

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
    count(const _Key& __k) const
    {
      pair<const_iterator, const_iterator> __p = equal_range(__k);
      const size_type __n = std::distance(__p.first, __p.second);
      return __n;
    }

  unsigned int
  _Rb_tree_black_count(const _Rb_tree_node_base* __node,
                      const _Rb_tree_node_base* __root) throw ();

  template<typename _Key, typename _Val, typename _KeyOfValue,
          typename _Compare, typename _Alloc>
    bool
    _Rb_tree<_Key,_Val,_KeyOfValue,_Compare,_Alloc>::__rb_verify() const
    {
      if (_M_impl._M_node_count == 0 || begin() == end())
    return _M_impl._M_node_count == 0 && begin() == end()
          && this->_M_impl._M_header._M_left == _M_end()
          && this->_M_impl._M_header._M_right == _M_end();

      unsigned int __len = _Rb_tree_black_count(_M_leftmost(), _M_root());
      for (const_iterator __it = begin(); __it != end(); ++__it)
    {
      _Const_Link_type __x = static_cast<_Const_Link_type>(__it._M_node);
      _Const_Link_type __L = _S_left(__x);
      _Const_Link_type __R = _S_right(__x);

      if (__x->_M_color == _S_red)
        if ((__L && __L->_M_color == _S_red)
        || (__R && __R->_M_color == _S_red))
          return false;

      if (__L && _M_impl._M_key_compare(_S_key(__x), _S_key(__L)))
        return false;
      if (__R && _M_impl._M_key_compare(_S_key(__R), _S_key(__x)))
        return false;

      if (!__L && !__R && _Rb_tree_black_count(__x, _M_root()) != __len)
        return false;
    }

      if (_M_leftmost() != _Rb_tree_node_base::_S_minimum(_M_root()))
    return false;
      if (_M_rightmost() != _Rb_tree_node_base::_S_maximum(_M_root()))
    return false;
      return true;
  }
}

#endif