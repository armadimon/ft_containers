#ifndef ITERATOR_HPP
# define ITERATOR_HPP
 
#include <iterator>
#include "utility.hpp"

namespace ft {

	struct  input_iterator_tag {};
	struct  output_iterator_tag {};
	struct  forward_iterator_tag       : public input_iterator_tag {};
	struct  bidirectional_iterator_tag : public forward_iterator_tag {};
	struct  random_access_iterator_tag : public bidirectional_iterator_tag {};


    template <typename _Category, typename _T, typename _Distance = ptrdiff_t,
            typename _pointer = _T *, typename _reference = _T &>
    struct iterator {
		typedef _T value_type;
		typedef _Distance difference_type;
		typedef _pointer pointer;
		typedef _reference reference;
		typedef _Category iterator_category;
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
	struct _iterator_traits_typedefs {};

	template <typename _Iter>
	struct _iterator_traits_typedefs<_Iter, true> {
	typedef typename _Iter::iterator_category iterator_category;
	typedef typename _Iter::value_type value_type;
	typedef typename _Iter::difference_type difference_type;
	typedef typename _Iter::pointer pointer;
	typedef typename _Iter::reference reference;
	};

	template <typename _Iter, bool>
	struct _iterator_traits {};

		template <typename _Iter>
		struct _iterator_traits<_Iter, true>
			: _iterator_traits_typedefs<
				_Iter, is_same<typename _Iter::iterator_category,
								input_iterator_tag>::value ||
							is_same<typename _Iter::iterator_category,
									output_iterator_tag>::value ||
							is_same<typename _Iter::iterator_category,
									forward_iterator_tag>::value ||
							is_same<typename _Iter::iterator_category,
									bidirectional_iterator_tag>::value ||
							is_same<typename _Iter::iterator_category,
									random_access_iterator_tag>::value> {};

	
	template <typename _Iter>
	struct iterator_traits
		: _iterator_traits<_Iter, _has_iterator_typedefs<_Iter>::value> {};

template <typename _T>
struct iterator_traits<_T *> {
	typedef random_access_iterator_tag iterator_category;
	typedef _T value_type;
	typedef ptrdiff_t difference_type;
	typedef _T *pointer;
	typedef _T &reference;
};

template <typename _T>
struct iterator_traits<const _T *> {
	typedef random_access_iterator_tag iterator_category;
	typedef typename std::remove_cv<_T>::type value_type;
	typedef ptrdiff_t difference_type;
	typedef const _T *pointer;
	typedef const _T &reference;
};

template <typename _Iter, bool = _has_iterator_typedefs<iterator_traits <_Iter> >::value>
struct is_iterator : public false_type {
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
struct is_random_access_iterator : public integral_constant<
	bool, is_same<typename is_iterator<_Iter>::category, random_access_iterator_tag>::value>
{};

};
#endif