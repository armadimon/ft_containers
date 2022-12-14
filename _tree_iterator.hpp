#ifndef _TREE_ITERATOR_HPP
# define _TREE_ITERATOR_HPP

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
  struct _Rb_tree_node : public _Rb_tree_node_base {
    typedef _Rb_tree_node<_Val>* _Link_type;

    _Val _M_value_field;

    _Val *__valptr() { return &_M_value_field; }
    const _Val *__valptr() const { return &_M_value_field; }
  };

  _Rb_tree_node_base*
  _Rb_tree_increment(_Rb_tree_node_base* __x)
  {
    if (__x->_M_right != 0) {
      __x = __x->_M_right;
      while (__x->_M_left != 0)
        __x = __x->_M_left;
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent;
      while (__x == __y->_M_right) {
        __x = __y;
        __y = __y->_M_parent;
      }
      if (__x->_M_right != __y)
        __x = __y;
    }
    return (__x);
  }

   const _Rb_tree_node_base*
  _Rb_tree_increment(const _Rb_tree_node_base* __x)
  {
    if (__x->_M_right != 0) {
      __x = __x->_M_right;
      while (__x->_M_left != 0)
        __x = __x->_M_left;
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent;
      while (__x == __y->_M_right) {
        __x = __y;
        __y = __y->_M_parent;
      }
      if (__x->_M_right != __y)
        __x = __y;
    }
    return (__x);
  }

   _Rb_tree_node_base*
  _Rb_tree_decrement(_Rb_tree_node_base* __x)
  {
    if (__x->_M_color == _S_red &&
        __x->_M_parent->_M_parent == __x)
      __x = __x->_M_right;
    else if (__x->_M_left != 0) {
      _Rb_tree_node_base* __y = __x->_M_left;
      while (__y->_M_right != 0)
        __y = __y->_M_right;
      __x = __y;
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent;
      while (__x == __y->_M_left) {
        __x = __y;
        __y = __y->_M_parent;
      }
      __x = __y;
    }
    return (__x);
  }

   const _Rb_tree_node_base*
  _Rb_tree_decrement(const _Rb_tree_node_base* __x)
  {
    if (__x->_M_color == _S_red &&
        __x->_M_parent->_M_parent == __x)
      __x = __x->_M_right;
    else if (__x->_M_left != 0) {
      _Rb_tree_node_base* __y = __x->_M_left;
      while (__y->_M_right != 0)
        __y = __y->_M_right;
      __x = __y;
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent;
      while (__x == __y->_M_left) {
        __x = __y;
        __y = __y->_M_parent;
      }
      __x = __y;
    }
    return (__x);
  }


  template<typename _Tp>
  struct _Rb_tree_iterator
  {
    typedef _Tp  value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t                  difference_type;

    typedef _Rb_tree_iterator<_Tp>        _Self;
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    typedef _Rb_tree_node<_Tp>*           _Link_type;

    _Rb_tree_iterator()
    : _M_node() { }

    explicit
    _Rb_tree_iterator(_Link_type __x)
    : _M_node(__x) { }

    reference
    operator*() const
    { return static_cast<_Link_type>(_M_node)->_M_value_field; }

    pointer
    operator->() const
    { return &(static_cast<_Link_type>
              (_M_node)->_M_value_field); }

    _Self&
    operator++() {
      _M_node = _Rb_tree_increment(_M_node);
      return *this;
    }

    _Self
    operator++(int) {
      _Self __tmp = *this;
      _M_node = _Rb_tree_increment(_M_node);
      return __tmp;
    }

    _Self&
    operator--() {
      _M_node = _Rb_tree_decrement(_M_node);
      return *this;
    }

    _Self
    operator--(int) {
      _Self __tmp = *this;
      _M_node = _Rb_tree_decrement(_M_node);
      return __tmp;
    }

    bool
    operator==(const _Self& __x) const
    { return _M_node == __x._M_node; }

    bool
    operator!=(const _Self& __x) const
    { return _M_node != __x._M_node; }

    _Base_ptr _M_node;
};

template<typename _Tp>
  struct _Rb_tree_const_iterator
  {
    typedef _Tp        value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;

    typedef _Rb_tree_iterator<_Tp> iterator;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t                  difference_type;

    typedef _Rb_tree_const_iterator<_Tp>        _Self;
    typedef _Rb_tree_node_base::_Const_Base_ptr _Base_ptr;
    typedef const _Rb_tree_node<_Tp>*           _Link_type;

    _Rb_tree_const_iterator()
    : _M_node() { }

    explicit
    _Rb_tree_const_iterator(_Link_type __x)
    : _M_node(__x) { }

    _Rb_tree_const_iterator(const iterator& __it)
    : _M_node(__it._M_node) { }

    iterator
    _M_const_cast() const
    { return iterator(static_cast<typename iterator::_Link_type>
          (const_cast<typename iterator::_Base_ptr>(_M_node))); }

    reference
    operator*() const
    { return static_cast<_Link_type>(_M_node)->_M_value_field; }

    pointer
    operator->() const
    { return addressof(static_cast<_Link_type>
              (_M_node)->_M_value_field); }

    _Self&
    operator++()
    {
  _M_node = _Rb_tree_increment(_M_node);
  return *this;
    }

    _Self
    operator++(int)
    {
  _Self __tmp = *this;
  _M_node = _Rb_tree_increment(_M_node);
  return __tmp;
    }

    _Self&
    operator--()
    {
  _M_node = _Rb_tree_decrement(_M_node);
  return *this;
    }

    _Self
    operator--(int)
    {
  _Self __tmp = *this;
  _M_node = _Rb_tree_decrement(_M_node);
  return __tmp;
    }

    bool
    operator==(const _Self& __x) const
    { return _M_node == __x._M_node; }

    bool
    operator!=(const _Self& __x) const
    { return _M_node != __x._M_node; }

    _Base_ptr _M_node;
  };

template<typename _Val>
  inline bool
  operator==(const _Rb_tree_iterator<_Val>& __x,
            const _Rb_tree_const_iterator<_Val>& __y)
  { return __x._M_node == __y._M_node; }

template<typename _Val>
  inline bool
  operator!=(const _Rb_tree_iterator<_Val>& __x,
            const _Rb_tree_const_iterator<_Val>& __y)
  { return __x._M_node != __y._M_node; }

void 
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

void 
_Rb_tree_rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base*const __y = __x->_M_left;
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


void
_Rb_tree_insert_and_rebalance(const bool __insert_left,
                              _Rb_tree_node_base* __x,
                              _Rb_tree_node_base* __p,
                              _Rb_tree_node_base& __header) {

    // initiate new node
    __x->_M_parent = __p;
    __x->_M_right = NULL;
    __x->_M_left = NULL;
    __x->_M_color = _S_red;

    // __p??? left??? __x??? ??????
    if (__insert_left) {
      __p->_M_left = __x;
      // ????????? ????????? ????????? ?????? ??????
      if (__p == &__header) {
        __header._M_parent = __x;
        __header._M_right = __x;
      } else if (__p == __header._M_left) {
        __header._M_left = __x;  // __p??? leftmost??? ?????? __x??? leftmost
      }
    } else {  // __p??? right??? __x ??????
      __p->_M_right = __x;
      if (__p == __header._M_right)
        __header._M_right = __x;  // __p??? rightmost??? ?????? __x??? rightmost
    }
// rebalance
// ????????? ????????? black??? ????????? reblancing?????? ?????????.
  _Rb_tree_node_base*& __root = __header._M_parent;
  while (__x != __root && __x->_M_parent->_M_color == _S_red) {
    // ???????????? ?????? ????????? ?????? ????????????.
    _Rb_tree_node_base* const __xgp = __x->_M_parent->_M_parent;
    if (__x->_M_parent == __xgp->_M_left) {
      // _x??? ????????? ???????????? ??????????????? 
      _Rb_tree_node_base* const __y = __xgp->_M_right;
      if (__y && __y->_M_color == _S_red) {
        // __x??? uncle ?????????_S_red(case 1)
        __x->_M_parent->_M_color = _S_black;
        __y->_M_color = _S_black;
        __xgp->_M_color = _S_red;
        __x = __xgp;
      } else {  // __x??? uncle ?????????_S_black (case 2)
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;
          _Rb_tree_rotate_left(__x, __root);
        }
        __x->_M_parent->_M_color = _S_black;  // case 3
        __xgp->_M_color = _S_red;
        _Rb_tree_rotate_right(__xgp, __root);
      }
    } else {
      // _x??? ????????? ???????????? ???????????????.
      _Rb_tree_node_base *const __y = __xgp->_M_left;
      if (__y && __y->_M_color == _S_red) {
        __x->_M_parent->_M_color = _S_black;
        __y->_M_color = _S_black;
        __xgp->_M_color = _S_red;
        __x = __xgp;
      } else {
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _Rb_tree_rotate_right(__x, __root);
        }
        __x->_M_parent->_M_color = _S_black;
        __xgp->_M_color = _S_red;
        _Rb_tree_rotate_left(__xgp, __root);
      }
    }
  }
  __root->_M_color = _S_black; // ?????? 2 : root node??? ?????? black??????.
}

_Rb_tree_node_base*
_Rb_tree_rebalance_for_erase(_Rb_tree_node_base* const __z,
                _Rb_tree_node_base& __header) throw () {
  // ????????? ??????????????? ?????? ?????????????????? ????????? ?????? ?????? ?????????????????? ???????????? ?????? ????????? ????????? ???????????????,
  // ?????? ???????????? ????????? ???????????? ???????????? ????????????. ????????? ????????? ????????? ????????? ????????? ?????? 1?????? ?????? ????????? ??????.
  _Rb_tree_node_base*& __root = __header._M_parent;
  _Rb_tree_node_base* __y = __z; //
  _Rb_tree_node_base* __x = 0;
  _Rb_tree_node_base* __x_parent = 0;
  if (__y->_M_left == 0) {
    // __z?????? ?????? ????????? ????????? ????????? ??????. __y == __z
    __x = __y->_M_right;
  }
  else {
    if (__y->_M_right == 0) { 
      // __z??? ????????? ????????? non-null child??? ????????? ?????? y == z.
      __x = __y->_M_left;     // __x is not null.
    }   
    else {                   // __z ??? ????????? ???????????? ??????.
      __y = __y->_M_right;   // __y ???__z??? successor??? ??????.
      while (__y->_M_left != 0)
        __y = __y->_M_left;
      __x = __y->_M_right;
    }
  }
  if (__y != __z) {          // ?????? ????????? ?????? y??? z??? successor??? ?????????
    __z->_M_left->_M_parent = __y; 
    __y->_M_left = __z->_M_left;
    if (__y != __z->_M_right) {
      __x_parent = __y->_M_parent;
      if (__x) __x->_M_parent = __y->_M_parent;
      __y->_M_parent->_M_left = __x;
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
    ft::swap(__y->_M_color, __z->_M_color);
    __y = __z;
    // __y now points to node to be actually deleted
  }
  else {    // __y == __z
    __x_parent = __y->_M_parent;
    if (__x)
      __x->_M_parent = __y->_M_parent;   
    if (__root == __z)
      __root = __x;
    else
    {
      if (__z->_M_parent->_M_left == __z)
        __z->_M_parent->_M_left = __x;
      else
        __z->_M_parent->_M_right = __x;
    }
    if (__header._M_left == __z)
    {
      if (__z->_M_right == 0)
        __header._M_left = __z->_M_parent;
      else
        __header._M_left = _Rb_tree_node_base::_S_minimum(__x);
    }
    if (__header._M_right == __z)
    {
      if (__z->_M_left == 0)
        __header._M_right = __z->_M_parent;
      else
        __header._M_right = _Rb_tree_node_base::_S_maximum(__x);
    }
  }

  // fix
  
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
      } else { //  ?????? ??????
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
    if (__x)
      __x->_M_color = _S_black;
  }
  return __y;
}
}

#endif