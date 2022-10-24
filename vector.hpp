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

	// Member functions

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

	vector(const vector& _x)
		:
		_a(_x._a),
		_begin(nullptr),
		_end(nullptr),
		_end_cap(nullptr)
	{
  		clear();
		size_type _n = _x.size();
		_begin = _a.allocate(_n);
		_end_cap = _begin + _n;
		_end = _begin;
  		this->_end = std::uninitialized_copy(_x._begin, _x._end, this->_begin);
	}

	vector<_T, Allocator>& operator=(const vector<_T, Allocator>& other) {
	if (this != &other) {
		if (this->_a != other._a) {
			clear();
			this->_a.deallocate(this->_begin, capacity());
			this->_begin = this->_end = this->_end_cap = NULL;
			this->_a = other._a;
		}
		assign(other._begin, other._end);
	}
		return *this;
	}

	~vector()
	{
		this->clear();
		if (this->_begin != nullptr)
			_a.deallocate(_begin, this->capacity());
	}
	
	template <typename InputIt>
	void assign(InputIt first, 	typename ft::enable_if<ft::is_input_iterator<InputIt>::value,
		InputIt>::type last) {
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
			_end = _begin + _new_size;
			std::uninitialized_copy(first, last, _begin);
		}
	}

	void assign(size_type __n, const_reference __u)
	{
		if (__n > capacity())
		{
			pointer		tmp_begin = _begin;
			pointer		tmp_end = _end;
			pointer		tmp_end_cap = _end_cap;
			pointer		tmp_begin2 = _begin;
	
			_begin = _a.allocate(__n);
			_end_cap = _begin + __n;
			_end = _begin;
			while (_end != _end_cap) {
				_a.construct(_end, __u);
				_end++;
			}
			_a.deallocate(tmp_begin2, tmp_end_cap - tmp_begin2);
		}
		else
		{
			_end = _begin + __n;
			std::uninitialized_copy(_begin, _end, _begin);
			pointer	tmp_begin =_begin;
			while (tmp_begin != _end)
			{
				_a.construct(tmp_begin, __u);
				tmp_begin++;
			}
		}
	}


	// Iterators
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

	// Capacity

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
        return *this->_begin;
    }
    
	const_reference front() const NOEXCEPT
    {
        return *this->_begin;
    }
    
	reference       back() NOEXCEPT
    {
        return *(this->_end - 1);
    }
    
	const_reference back()  const NOEXCEPT
    {
        return *(this->_end - 1);
    }

	reference operator[](size_type _n) NOEXCEPT {
		return (this->_begin[_n]);
	}

	// Modifiers

	iterator insert( const_iterator __position, const _T& __X )
	{
		pointer __p = this->_begin + (__position - begin());
		size_type __p_len = &(*__p) - _begin;
		if (this->_end < this->_end_cap)
		{
			if (__p == this->_end) {
				_a.construct(_end, __X);
				_end++;
			}
			else {
				pointer __i = this->_end;
				for (; __i >= this->_begin; --__i) {
					this->_a.construct(__i, *(__i - 1));
				}
				_a.construct(__p, __X);
				_end++;
			}
		}
		else
		{
			pointer		tmp_begin = _begin;
			pointer		tmp_end = _end;
			pointer		tmp_end_cap = _end_cap;
			pointer		tmp_begin2 = _begin;
			
			size_t new_cap = (this->size() * 2 > 0) ? this->size() * 2 : 1; 
			_begin = _a.allocate(new_cap);
			_end = _begin;
			_end_cap = _begin + new_cap;
			while (tmp_begin != tmp_end) {
				if (tmp_begin == __p) {
					_a.construct(_end, __X);
					_end++;
				}
				_a.construct(_end, *tmp_begin);
				_end++;
				tmp_begin++;
			}
			_a.deallocate(tmp_begin2, tmp_end_cap - tmp_begin2);
		}
		return (iterator(_begin +__p_len));
	}

	iterator insert( const_iterator __position, size_type __n, const _T& __X )
	{
		pointer __p = this->_begin + (__position - begin());
		size_type __p_len = &(*__p) - _begin;
		if (__n > 0)
		{
			pointer _old_p = __p;
			if (__n <= static_cast<size_type>(this->_end_cap - this->_end)) {
				if (__p == this->_end) {
					while (__n--) {
						_a.construct(_end, __X);
						_end++;
					}
				}
				else {
					pointer __i = this->_end + __n - 1;
					for (; __i >= this->_begin + __n; --__i) {
						this->_a.construct(__i, *(__i - __n));
					}
					while (__n--) {
						_a.construct(__p, __X);
						_end++;
						__p++;
					}
					__p = _old_p;
				}
			}
			else {
				pointer		tmp_begin = _begin;
				pointer		tmp_end = _end;
				pointer		tmp_end_cap = _end_cap;
				pointer		tmp_begin2 = _begin;

				size_t new_cap = (this->size() * 2 > 0) ? (this->size() * 2) : __n; 
				_begin = _a.allocate(new_cap);
				_end = _begin;
				_end_cap = _begin + new_cap;
				while (tmp_begin != tmp_end) {
					if (tmp_begin == __p) {
						while (__n--) {
							_a.construct(_end, __X);
							_end++;
						}
					}
					_a.construct(_end, *tmp_begin);
					_end++;
					tmp_begin++;
				}
				_a.deallocate(tmp_begin2, tmp_end_cap - tmp_begin2);
			}
		}
		return (iterator(_begin +__p_len));
	}

	template< typename InputIt >
	iterator insert(const_iterator _position,
					InputIt _first, typename enable_if<is_input_iterator<InputIt>::value, InputIt>::type _last)
	{
		if (_begin == nullptr)
			throw std::bad_alloc();
		pointer __p = this->_begin + (_position - begin());
		size_type __p_len = &(*__p) - _begin;
		difference_type __n = ft::_distance(_first, _last);
		if (__n > 0)
		{
			pointer _old_p = __p;
			if (__n <= static_cast<size_type>(this->_end_cap - this->_end)) {
				if (__p == this->_end) {
					while (_first != _last) {
						_a.construct(_end, *_first);
						_first++;
					}
				}
				else {
					pointer __i = this->_end + __n - 1;
					for (; __i >= this->_begin + __n; --__i) {
						this->_a.construct(__i, *(__i - __n));
					}
					while (_first != _last) {
						_a.construct(_end, *_first);
						_first++;
						_end++;
						__p++;
					}
					__p = _old_p;
				}
			}
			else {
				pointer		tmp_begin = _begin;
				pointer		tmp_end = _end;
				pointer		tmp_end_cap = _end_cap;
				pointer		tmp_begin2 = _begin;

				size_t new_cap = (this->size() > 0) ? (this->size() * 2) : __n;
				_begin = _a.allocate(new_cap);
				_end = _begin;
				_end_cap = _begin + new_cap;
				while (tmp_begin != tmp_end) {
					if (tmp_begin == __p) {
						while (_first != _last) {
							_a.construct(_end, *_first);
							_first++;
							_end++;
						}
					}
					_a.construct(_end, *tmp_begin);
					_end++;
					tmp_begin++;
				}
				_a.deallocate(tmp_begin2, tmp_end_cap - tmp_begin2);
			}
		}
		return (iterator(_begin +__p_len));
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


	void swap( vector& other )
	{
		pointer	tmp_begin = _begin;
		pointer tmp_end = _end;
		pointer tmp_end_cap = _end_cap;
		allocator_type tmp_a = _a;

		this->_begin = other._begin;
		this->_end = other._end;
		this->_end_cap = other._end_cap;
		this->_a = other._a;

		other._begin = tmp_begin;
		other._end = tmp_end;
		other._end_cap = tmp_end_cap;
		other._a = tmp_a;
	}

	void clear() {
		size_type tmp_size = size();
		for (size_type _i = 0; _i < tmp_size; _i++)
		{
			_end--;
			_a.destroy(_end);
		}
	}

	iterator erase(iterator _position)
	{
		pointer __p = this->_begin + (_position - begin());
		this->_a.destroy(__p);
		std::uninitialized_copy(__p + 1, this->_end--, __p);
  		return (iterator(this->_begin + (_position - begin())));
	}

	iterator erase(iterator _first, iterator _last)
	{
		pointer __p = this->_begin + (_first - begin());
		pointer __p_last = this->_begin + (_last - begin());
		// pointer __tmp_p_last = this->_begin + (_last - begin());
		difference_type __diff = _first - begin();
		// std::cout << __diff << std::endl;
		pointer _old_p = __p;
		if (_first != _last)
		{
			while (__p_last-- != __p)
			{
				this->_a.destroy(__p_last);
				_a.construct(__p_last, *(--this->_end));
			}
		}
		else
			return (_last);
		return (iterator(this->_begin + __diff));
	}

	void	resize (size_type _n, value_type _x = value_type())
	{
		if (_n > this->max_size())
			throw (std::length_error("vector::resize"));
		else if (_n < this->size())
		{
			while (this->size() > _n)
			{
				_end--;
				_a.destroy(_end);
			}
		}
		else
		{
			if (_n > this->capacity())
				this->reserve(this->capacity() * 2);
			this->insert(this->end(), _n - this->size(), _x);
		}
	}

	void	pop_back()
	{
		_end--;
		_a.destroy(_end);
	}

	
	private:
		allocator_type _a;
		pointer _begin;
		pointer _end;
		pointer _end_cap;

   }; //end vector class


};

namespace std {
	template <class _T, class Allocator>
		void swap(ft::vector<_T,Allocator>& x, ft::vector<_T,Allocator>&y)
	{
		x.swap(y);
	};
}

#endif