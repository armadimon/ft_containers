#ifndef ITERATOR_HPP
# define ITERATOR_HPP
 
#include <iterator>
#include "utility.hpp"

namespace ft {

	// struct  input_iterator_tag {};
	// struct  output_iterator_tag {};
	// struct  forward_iterator_tag       : public input_iterator_tag {};
	// struct  bidirectional_iterator_tag : public forward_iterator_tag {};
	// struct  random_access_iterator_tag : public bidirectional_iterator_tag {};

    template <typename _Category, typename _T, typename _Distance = ptrdiff_t,
            typename _pointer = _T *, typename _reference = _T &>
    struct iterator {
		typedef _Category iterator_category;
		typedef _T value_type;
		typedef _Distance difference_type;
		typedef _pointer pointer;
		typedef _reference reference;
    };

	template <typename>
	struct void_t {
		typedef void type;
	};

	template <typename _T>
	struct _has_iterator_typedefs {
		private:
		struct __two {
			char __x;
			char __y;
		};

		template <typename _U>
		static __two __test(...){};

		template <typename _U>
		static char __test(
			typename ft::void_t<typename _U::iterator_category>::type * = 0,
			typename ft::void_t<typename _U::difference_type>::type * = 0,
			typename ft::void_t<typename _U::value_type>::type * = 0,
			typename ft::void_t<typename _U::reference>::type * = 0,
			typename ft::void_t<typename _U::pointer>::type * = 0){};

		public:
		static const bool value = sizeof(__test<_T>(0, 0, 0, 0, 0)) == 1;
	};

	template <typename _Iter, bool>
	struct _iterator_traits {};

		template <typename _Iter>
		struct _iterator_traits<_Iter, true> {
		typedef typename _Iter::iterator_category iterator_category;
		typedef typename _Iter::value_type value_type;
		typedef typename _Iter::difference_type difference_type;
		typedef typename _Iter::pointer pointer;
		typedef typename _Iter::reference reference;
	};

	
	template <typename _Iter>
	struct iterator_traits
		: _iterator_traits<_Iter, _has_iterator_typedefs<_Iter>::value> {
		};

template <typename _T>
struct iterator_traits<_T *> {
	typedef std::random_access_iterator_tag iterator_category;
	typedef _T value_type;
	typedef ptrdiff_t difference_type;
	typedef _T *pointer;
	typedef _T &reference;
};

template <typename _T>
struct iterator_traits<const _T *> {
	typedef std::random_access_iterator_tag iterator_category;
	typedef typename std::remove_cv<_T>::type value_type;
	typedef ptrdiff_t difference_type;
	typedef const _T *pointer;
	typedef const _T &reference;
};

template <typename _Iter, bool = _has_iterator_typedefs<iterator_traits <_Iter> >::value>
struct is_iterator : public false_type {
  typedef void category;
};

template <typename _Iter>
struct is_iterator<_Iter, true> : public true_type {
  typedef typename _Iter::iterator_category category;
};

template <typename _T>
struct is_iterator<_T *, true> : public true_type {
  typedef typename iterator_traits<_T *>::iterator_category category;
};

template <typename _Iter>
struct is_input_iterator : public integral_constant<bool,
	(is_same<typename is_iterator<_Iter>::category,
	std::random_access_iterator_tag>::value ||
	is_same<typename is_iterator<_Iter>::category,
	std::bidirectional_iterator_tag>::value ||
	is_same<typename is_iterator<_Iter>::category,
	std::forward_iterator_tag>::value ||
	is_same<typename is_iterator<_Iter>::category,
	std::input_iterator_tag>::value)>
{};

template <typename _Iter>
struct is_forward_iterator : public integral_constant<bool,
	(is_same<typename is_iterator<_Iter>::category,
	std::random_access_iterator_tag>::value ||
	is_same<typename is_iterator<_Iter>::category,
	std::bidirectional_iterator_tag>::value ||
	is_same<typename is_iterator<_Iter>::category,
	std::forward_iterator_tag>::value)>
{};

template <typename _Iter>
struct is_output_iterator : public integral_constant<
	bool, is_same<typename is_iterator<_Iter>::category, std::output_iterator_tag>::value>
{};

template <typename _Iter>
struct is_random_access_iterator : public integral_constant<
	bool, is_same<typename is_iterator<_Iter>::category, std::random_access_iterator_tag>::value>
{};

template <class _InputIter>
typename iterator_traits<_InputIter>::difference_type
_distance(_InputIter _first, _InputIter _last, typename ft::enable_if<ft::is_input_iterator<_InputIter>::value,
		_InputIter>::type* = 0)
{
    typename iterator_traits<_InputIter>::difference_type _r(0);
    for (; _first != _last; ++_first)
        ++_r;
    return _r;
}

template <class _Iter>
class reverse_iterator
    : public iterator<typename iterator_traits<_Iter>::iterator_category,
                      typename iterator_traits<_Iter>::value_type,
                      typename iterator_traits<_Iter>::difference_type,
                      typename iterator_traits<_Iter>::pointer,
                      typename iterator_traits<_Iter>::reference>
{
private:
    _Iter __t;  
protected:
    _Iter current;
public:
    typedef _Iter                                            iterator_type;
    typedef typename iterator_traits<_Iter>::difference_type difference_type;
    typedef typename iterator_traits<_Iter>::reference       reference;
    typedef typename iterator_traits<_Iter>::pointer         pointer;

    reverse_iterator() : __t(), current() {}
    explicit reverse_iterator(_Iter __x) : __t(__x), current(__x) {}
    template <class _Up>
        reverse_iterator(const reverse_iterator<_Up>& __u) : __t(__u.base()), current(__u.base()) {}
    template <class _Up>
        reverse_iterator& operator=(const reverse_iterator<_Up>& __u)
            { __t = current = __u.base(); return *this; }
    _Iter base() const {return current;}
    reference operator*() const {_Iter __tmp = current; return *--__tmp;}
    pointer  operator->() const {return &(operator*());}
    reverse_iterator& operator++() {--current; return *this;}
    reverse_iterator  operator++(int) {reverse_iterator __tmp(*this); --current; return __tmp;}
    reverse_iterator& operator--() {++current; return *this;}
    reverse_iterator  operator--(int) {reverse_iterator __tmp(*this); ++current; return __tmp;}
    reverse_iterator  operator+ (difference_type __n) const {return reverse_iterator(current - __n);}
    reverse_iterator& operator+=(difference_type __n) {current -= __n; return *this;}
    reverse_iterator  operator- (difference_type __n) const {return reverse_iterator(current + __n);}
    reverse_iterator& operator-=(difference_type __n) {current += __n; return *this;}
    reference         operator[](difference_type __n) const {return *(*this + __n);}
};

template <class _Iter1, class _Iter2>
bool
operator==(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
{
    return __x.base() == __y.base();
}

template <class _Iter1, class _Iter2>
bool
operator<(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
{
    return __x.base() > __y.base();
}

template <class _Iter1, class _Iter2>
bool
operator!=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
{
    return __x.base() != __y.base();
}

template <class _Iter1, class _Iter2>
bool
operator>(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
{
    return __x.base() < __y.base();
}

template <class _Iter1, class _Iter2>
bool
operator>=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
{
    return __x.base() <= __y.base();
}

template <class _Iter1, class _Iter2>
bool
operator<=(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
{
    return __x.base() >= __y.base();
}


template <class _Iter1, class _Iter2>
typename reverse_iterator<_Iter1>::difference_type
operator-(const reverse_iterator<_Iter1>& __x, const reverse_iterator<_Iter2>& __y)
{
    return __y.base() - __x.base();
}

template <class _Iter>
reverse_iterator<_Iter>
operator+(typename reverse_iterator<_Iter>::difference_type __n, const reverse_iterator<_Iter>& __x)
{
    return reverse_iterator<_Iter>(__x.base() - __n);
}

};

#endif