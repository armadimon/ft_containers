#ifndef VECTOR_HPP
# define VECTOR_HPP

#include "iterator.hpp"
#include <memory>

namespace ft {

template<typename _Iter>
class vector_iterator {
	public:
		typedef _Iter iterator_type;
		typedef typename ft::iterator_traits<iterator_type>::iterator_category
			iterator_category;
		typedef typename ft::iterator_traits<iterator_type>::value_type value_type;
		typedef typename ft::iterator_traits<iterator_type>::difference_type
			difference_type;
		typedef typename ft::iterator_traits<iterator_type>::pointer pointer;
		typedef typename ft::iterator_traits<iterator_type>::reference reference;
	private:
		iterator_type _i;

	public:
		vector_iterator() NOEXCEPT {}
		vector_iterator(iterator_type _x) NOEXCEPT : _i(_x) {}

		template <typename _Up>
		vector_iterator(const vector_iterator<_Up>& _u,
		typename enable_if<is_random_access_iterator<_Up>::value>::type* = 0) NOEXCEPT
		: _i(_u.base()) {}
		~vector_iterator() {}

	reference operator*() const NOEXCEPT {
	return *_i;
	}

	pointer  operator->() const NOEXCEPT {
	return _i;
	}

	vector_iterator& operator++() NOEXCEPT {
	++_i;
	return *this;
	}

	vector_iterator operator++(int) NOEXCEPT {
	vector_iterator _tmp(*this);
	++(*this);
	return _tmp;
	}

	vector_iterator& operator--() NOEXCEPT {
	--_i;
	return *this;
	}         

	vector_iterator operator--(int) NOEXCEPT {
	vector_iterator _tmp(*this);
	--(*this);
	return _tmp;
	}

	vector_iterator operator+ (difference_type _n) const NOEXCEPT {
	vector_iterator _w(*this);
	_w += _n;
	return _w;
	}
	
	vector_iterator& operator+=(difference_type _n) NOEXCEPT {
	_i += _n;
	return *this;
	}
	
	vector_iterator operator- (difference_type _n) const NOEXCEPT {
	vector_iterator _w(*this);
	_w -= _n;
	return _w;
	}
	
	vector_iterator& operator-=(difference_type _n) NOEXCEPT {
	_i -= _n;
	return *this;
	}
	reference    operator[](difference_type _n) const NOEXCEPT {
	return _i[_n];
	}

	private:
		iterator_type base() const NOEXCEPT {return _i;}
};

template <class _Iter1, class _Iter2>
bool
operator==(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT
{
	return __x.base() == __y.base();
}

template <class _Iter1, class _Iter2>
bool
operator<(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT {
	return __x.base() < __y.base();
}

template <class _Iter1, class _Iter2>
bool
operator!=(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT
{
	return !(__x == __y);
}

template <class _Iter1, class _Iter2>
bool
operator>(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT
{
	return __y < __x;
}

template <class _Iter1, class _Iter2>
bool
operator>=(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT
{
	return !(__x < __y);
}

template <class _Iter1, class _Iter2>
bool
operator<=(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT
{
	return !(__y < __x);
}

template<typename _T, typename Allocator = std::allocator<_T> >
class vector {
	public:
		typedef Allocator                               allocator_type;
		typedef _T                                      value_type;
		typedef value_type&                              reference;
		typedef const value_type&                        const_reference;
		typedef typename allocator_type::size_type       size_type;
		typedef typename allocator_type::difference_type difference_type;
		typedef typename allocator_type::pointer         pointer;
		typedef typename allocator_type::const_pointer   const_pointer;
		typedef vector_iterator<pointer>				iterator;
		typedef vector_iterator<const_pointer>			const_iterator;

	//constructor

	explicit vector (const allocator_type& _alloc = allocator_type())
	:
		_a(_alloc),
		_begin(nullptr),
		_end(nullptr),
		_end_cap(nullptr)
	{}

	explicit vector (size_type _n, 
		const value_type& _x = value_type(),
		const allocator_type& _alloc = allocator_type())
		:
		_end_cap(nullptr),
		_begin(nullptr),
		_end(nullptr),
		_a(_alloc)
	{
		_begin = _a.allocate(_n);
		_end_cap = _begin + _n;
		_end = _begin;
		while (_n--)
		{
		_a.construct(_end, _x);
		_end++;
		}
	}

	iterator begin() NOEXCEPT { return (this->_begin); };

	const_iterator begin() const { return (_begin); }

	iterator end() { return this->__end_; }

  	const_iterator end() const { return this->__end_; }

    bool empty() const NOEXCEPT {return this->_begin == this->_end;}

	size_type	size() const NOEXCEPT
		{return static_cast<size_type>(this->_end - this->_begin);}

    size_type capacity() const NOEXCEPT
        {return static_cast<size_type>(this->_end_cap - this->_begin);}

	size_type max_size() const {
    	return std::min(
		this->_a.max_size(),
        static_cast<size_type>(std::numeric_limits<difference_type>::max()));}

	// template<typename _T>
	// void	reserve(size_type new_cap) {
	// 	if (new_cap > max_size())
	// 		throw std::length_error("vector");
	// 	if (new_cap > capacity())
	// 	{
	// 		allocator_type _tmp_a = _a;

	// 		_begin = _a.allocate(_n);
	// 		_end_cap = _begin + _n;
	// 		_end = _begin;
	// 		while (_n--)
	// 		{
	// 			_a.construct(_end, _x);
	// 			_end++;
	// 		}
	// 	}
	// }

	reference operator[](size_type _n) NOEXCEPT {
		return (this->_begin[_n]);
	}

	private:
		allocator_type _a;
		pointer _begin;
		pointer _end;
		pointer _end_cap;

   };
};

#endif