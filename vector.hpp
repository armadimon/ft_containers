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

		iterator_type base() const NOEXCEPT {return _i;}

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

template <typename _Iter1, typename _Iter2>
bool
operator<=(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT
{
	return !(__y < __x);
}

template <typename _Iter1, typename _Iter2>
typename vector_iterator<_Iter1>::difference_type
operator-(const vector_iterator<_Iter1>& __x, const vector_iterator<_Iter2>& __y) NOEXCEPT
{
    return __x.base() - __y.base();
}

template <class _Iter>
vector_iterator<_Iter>
operator+(typename vector_iterator<_Iter>::difference_type __n,
          vector_iterator<_Iter> __x) NOEXCEPT
{
    __x += __n;
    return __x;
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
    	typedef ft::reverse_iterator<iterator>          reverse_iterator;
    	typedef ft::reverse_iterator<const_iterator>    const_reverse_iterator;

	//constructor

	explicit vector (const allocator_type& _alloc = allocator_type())
	:
		_a(_alloc),
		_begin(nullptr),
		_end(nullptr),
		_end_cap(nullptr)
	{}

	template< typename InputIt >
	vector( InputIt first,
	typename enable_if<is_input_iterator<InputIt>::value,
                        // !is_forward_iterator<InputIt>::value,
                       InputIt>::type last, 
        const Allocator& _alloc = Allocator())
		:
		_a(_alloc)
	{
		size_type _n = static_cast<size_type>(ft::_distance(first, last));
		std::cout << "range " << _n << std::endl;
		InputIt tmp_first = first;
		_begin = _a.allocate(_n);
		_end_cap = _begin + _n;
		_end = _begin;
		while (_n--) {
			_a.construct(_end, *tmp_first);
			_end++;
			tmp_first++;
		}
	}
	
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
		while (_n--) {
			_a.construct(_end, _x);
			_end++;
		}
	}

	~vector()
	{
		if (_begin != nullptr) {
			_a.deallocate(_begin, _end_cap - _begin);
		}
	}
	iterator begin() NOEXCEPT { return (this->_begin); };

	const_iterator begin() const { return (_begin); }

	iterator end() { return this->_end; }

  	const_iterator end() const { return this->_end; }

	reverse_iterator rbegin() { return reverse_iterator(end()); }

	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}
	reverse_iterator rend() { return reverse_iterator(begin()); }

	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	}

    bool empty() const NOEXCEPT {return this->_begin == this->_end;}

	size_type	size() const NOEXCEPT
		{return static_cast<size_type>(this->_end - this->_begin);}

    size_type capacity() const NOEXCEPT
        {return static_cast<size_type>(this->_end_cap - this->_begin);}
	
	size_type max_size() const {
    	return std::min(
		this->_a.max_size(),
        static_cast<size_type>(std::numeric_limits<difference_type>::max()));}

	void	reserve(size_type new_cap) {
		if (new_cap > max_size())
			throw std::length_error("vector");
		if (new_cap > capacity()) {
			pointer		tmp_begin = _begin;
			pointer		tmp_end = _end;
			pointer		tmp_end_cap = _end_cap;
			pointer		tmp_begin2 = _begin;
			
			// std::cout << "reserve "<< &_begin << " == " << &tmp_begin << " == " << &tmp_begin2 << std::endl;
			_begin = _a.allocate(new_cap);
			_end_cap = _begin + new_cap;
			_end = _begin;
			while (tmp_begin != tmp_end) {
				_a.construct(_end, *tmp_begin);
				_end++;
				tmp_begin++;
			}
			_a.deallocate(tmp_begin2, tmp_end_cap - tmp_begin2);
		}
	}

	template <typename T>
	void push_back( const T& value)
	{
		if (size() + 1 > capacity()) {
			if (capacity() == 0)
				reserve(capacity() + 1);
			else
				reserve(capacity() * 2);
		}
		_a.construct(_end, value);
		_end++;
	}

	template <typename InputIt>
	void assign(InputIt first, InputIt last,
		typename ft::enable_if<ft::is_input_iterator<InputIt>::value,
		InputIt>::type* = 0) {
		size_type _new_size = static_cast<size_type>(ft::_distance(first, last));
		if (_new_size > capacity())
		{
			pointer		tmp_begin = _begin;
			pointer		tmp_end = _end;
			pointer		tmp_end_cap = _end_cap;
			pointer		tmp_begin2 = _begin;

			// std::cout << "reserve "<< &_begin << " == " << &tmp_begin << " == " << &tmp_begin2 << std::endl;
			_begin = _a.allocate(_new_size);
			_end_cap = _begin + _new_size;
			_end = _begin;
			while (first != last) {
				_a.construct(_end, *first);
				_end++;
				first++;
			}
			_a.deallocate(tmp_begin2, tmp_end_cap - tmp_begin2);
		}
		else
		{
			InputIt __mid = last;
			bool __growing = false;
			if (_new_size > size())
			{
				__growing = true;
				__mid =  first;
				_VSTD::advance(__mid, size());
			}
			pointer __m = _VSTD::copy(first, __mid, this->_begin);
			if (__growing)
				__construct_at_end(__mid, last, _new_size - size());
			else
				this->__destruct_at_end(__m);

			}
	}

	// template <typename T>
	// void assign( size_type count, const T& value ) {

	// }

	// element access

	reference at( size_type pos )
	{
		if (pos >= size())
        	throw std::out_of_range("vector");
		return (this->_begin[pos]);
	}
	const_reference at( size_type pos ) const
	{
		if (pos >= size())
        	throw std::out_of_range("vector");
		return (this->_begin[pos]);
	}

	reference       front() NOEXCEPT
    {
        _LIBCPP_ASSERT(!empty(), "front() called for empty vector");
        return *this->_begin;
    }
    const_reference front() const NOEXCEPT
    {
        _LIBCPP_ASSERT(!empty(), "front() called for empty vector");
        return *this->_begin;
    }
    reference       back() NOEXCEPT
    {
        _LIBCPP_ASSERT(!empty(), "back() called for empty vector");
        return *(this->__end_ - 1);
    }
    const_reference back()  const NOEXCEPT
    {
        _LIBCPP_ASSERT(!empty(), "back() called for empty vector");
        return *(this->__end_ - 1);
    }

    // value_type*       data() NOEXCEPT
    //     {return (this->_begin);}
	
    // const value_type* data() const NOEXCEPT
    //     {return (this->_begin);}

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