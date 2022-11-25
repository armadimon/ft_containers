#ifndef MAP_HPP
# define MAP_HPP

#include "iterator.hpp"
#include "_tree.hpp"

namespace ft
{

template <class _Key, class _Tp, class _Compare = std::less<_Key>,
          class _Alloc = std::allocator<pair<const _Key, _Tp> > >
class map
{

public:
  // typedefs:
  typedef _Key                  key_type;
  typedef _Tp                   data_type;
  typedef _Tp                   mapped_type;
  typedef pair<const _Key, _Tp> value_type;
  typedef _Compare              key_compare;
    
  class value_compare
    : public std::binary_function<value_type, value_type, bool> {
  friend class map<_Key,_Tp,_Compare,_Alloc>;
  protected :
    _Compare comp;
    value_compare(_Compare __c) : comp(__c) {}
  public:
    bool operator()(const value_type& __x, const value_type& __y) const {
      return comp(__x.first, __y.first);
    }
  };

private:
  typedef _Rb_tree<key_type, value_type, 
                   ft::_Select1st<value_type>, key_compare, _Alloc> _Rep_type;
  _Rep_type _M_t;
public:
  typedef typename _Rep_type::pointer pointer;
  typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  typedef typename _Rep_type::iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;
  typedef typename _Rep_type::allocator_type allocator_type;

  // allocation/deallocation

  map() : _M_t(_Compare(), allocator_type()) {}
  explicit map(const _Compare& __comp,
               const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) {}

  template <class _InputIterator>
  map(_InputIterator __first, _InputIterator __last)
    : _M_t(_Compare(), allocator_type())
    { _M_t._M_insert_unique(__first, __last); }

  template <class _InputIterator>
  map(_InputIterator __first, _InputIterator __last, const _Compare& __comp,
      const allocator_type& __a = allocator_type())
    : _M_t(__comp, __a) { _M_t._M_insert_unique(__first, __last); }
  map(const map<_Key,_Tp,_Compare,_Alloc>& __x) : _M_t(__x._M_t) {}

  map<_Key,_Tp,_Compare,_Alloc>&
  operator=(const map<_Key, _Tp, _Compare, _Alloc>& __x)
  {
    _M_t = __x._M_t;
    return *this; 
  }

  // accessors:

  key_compare key_comp() const { return _M_t.key_comp(); }
  value_compare value_comp() const { return value_compare(_M_t.key_comp()); }
  allocator_type get_allocator() const { return _M_t.get_allocator(); }

  iterator begin() { return _M_t.begin(); }
  const_iterator begin() const { return _M_t.begin(); }
  iterator end() { return _M_t.end(); }
  const_iterator end() const { return _M_t.end(); }
  reverse_iterator rbegin() { return _M_t.rbegin(); }
  const_reverse_iterator rbegin() const { return _M_t.rbegin(); }
  reverse_iterator rend() { return _M_t.rend(); }
  const_reverse_iterator rend() const { return _M_t.rend(); }
  bool empty() const { return _M_t.empty(); }
  size_type size() const { return _M_t.size(); }
  size_type max_size() const { return _M_t.max_size(); }
  _Tp& operator[](const key_type& __k) {
    iterator __i = lower_bound(__k);
    // __i->first is greater than or equivalent to __k.
    if (__i == end() || key_comp()(__k, (*__i).first))
      __i = insert(__i, value_type(__k, _Tp()));
    return (*__i).second;
  }
  void swap(map<_Key,_Tp,_Compare,_Alloc>& __x) { _M_t.swap(__x._M_t); }

  // insert/erase

  pair<iterator,bool> insert(const value_type& __x) 
    { return _M_t._M_insert_unique(__x); }
  iterator insert(iterator position, const value_type& __x)
    { return _M_t._M_insert_unique_hint(position, __x); }
  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last) {
    _M_t._M_insert_unique(__first, __last);
  }

  void erase(iterator __position) { _M_t.erase(__position); }
  size_type erase(const key_type& __x) { return _M_t.erase(__x); }
  void erase(iterator __first, iterator __last)
    { _M_t.erase(__first, __last); }
  void clear() { _M_t.clear(); }

  // map operations:


  iterator find(const key_type& __x) { return _M_t.find(__x); }
  const_iterator find(const key_type& __x) const { return _M_t.find(__x); }
  size_type count(const key_type& __x) const {
    return _M_t.find(__x) == _M_t.end() ? 0 : 1; 
  }

  mapped_type& at(const key_type& __k) {
    iterator __i = lower_bound(__k);
    if (!key_comp()(__k, (*__i).first))
      return ((*__i).second);
    else
      throw(std::out_of_range("map::at"));
  }

  const mapped_type& at(const key_type& __k) const {
    const_iterator __i = lower_bound(__k);
    if (!key_comp()(__k, (*__i).first))
      return ((*__i).second);
    else
      throw(std::out_of_range("map::at"));
  }

  iterator lower_bound(const key_type& __x) {return _M_t.lower_bound(__x); }
  const_iterator lower_bound(const key_type& __x) const {
    return _M_t.lower_bound(__x); 
  }
  iterator upper_bound(const key_type& __x) {return _M_t.upper_bound(__x); }
  const_iterator upper_bound(const key_type& __x) const {
    return _M_t.upper_bound(__x); 
  }
  
  pair<iterator,iterator> equal_range(const key_type& __x) {
    return _M_t.equal_range(__x);
  }
  pair<const_iterator,const_iterator> equal_range(const key_type& __x) const {
    return _M_t.equal_range(__x);
  }

};

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator==(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return (__x.size() == __y.size() &&
          ft::equal(__x.begin(), __x.end(), __y.begin()));
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return ft::lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
                                     __y.end());
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator!=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__x == __y);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                      const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return __y < __x;
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator<=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__y < __x);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline bool operator>=(const map<_Key,_Tp,_Compare,_Alloc>& __x, 
                       const map<_Key,_Tp,_Compare,_Alloc>& __y) {
  return !(__x < __y);
}

template <class _Key, class _Tp, class _Compare, class _Alloc>
inline void swap(ft::map<_Key,_Tp,_Compare,_Alloc>& __x, 
                 ft::map<_Key,_Tp,_Compare,_Alloc>& __y) {
  __x.swap(__y);
}

}

#endif /* _CPP_BITS_STL_MAP_H */