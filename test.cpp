
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

#include <cstddef>
#include <iterator>
#include <iostream>

struct Add{
	Add(int toAdd) : _toAdd(toAdd) {} // default constructor
	int operator()(int x){
		return x + _toAdd;
	}
	private:
	int _toAdd;
};

// int main(){
// 	Add add_42(42); // state 저장
// 	Add add_13(13);
// 	int result_42 = add_42(2);
//         std::cout << result_42 << " ";
// 	int result_13 = add_13(2);
//         std::cout << result_13 << " ";
// 	std::vector<int> v(10, 10);   
//     for (std::vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
//         std::cout << *iter << " ";
//     }
// 	std::transform(v.begin(), v.end(), v.begin(), add_42);
//     for (std::vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
//         std::cout << *iter << " ";
//     }

// 	std::vector<int> v2(10, 10);

// 	std::vector<int>::iterator iter1 = v2.begin();
// 	std::vector<int>::iterator iter2 = v2.end();

// 	std::vector<int> v3(2, 10);

// 	v3.assign(iter1, iter2); // assign에서 range를 사용하는 문법은 assign(inputiter 1, inputiter 2)이고, 여기서는 pointer로 특수화된 vector의 iter가 random access iter tag를 가지고 있기 때문에 iterator의 상속 구조에 따라서 inputiterator로도 동작 할 수 있는 것이다.
// 	for (std::vector<int>::iterator iter = v3.begin(); iter != v3.end(); iter++) {
// 		std::cout << *iter << " ";
//     }
// }

namespace ft
{

struct nothing {};
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};

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
	struct __has_iterator_typedefs {
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
	struct __iterator_traits_typedefs {};

	template <typename _Iter>
	struct __iterator_traits_typedefs<_Iter, true> {
	typedef typename _Iter::iterator_category iterator_category;
	typedef typename _Iter::value_type value_type;
	typedef typename _Iter::difference_type difference_type;
	typedef typename _Iter::pointer pointer;
	typedef typename _Iter::reference reference;
	};

	template <typename _Iter, bool>
	struct __iterator_traits {};




template <typename T, typename U>
struct is_same : public std::false_type {};

template <typename T>
struct is_same<T, T> : public std::true_type {};

	template <typename _Iter>
	struct __iterator_traits<_Iter, true>
		: __iterator_traits_typedefs<
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
		: __iterator_traits<_Iter, __has_iterator_typedefs<_Iter>::value> {};


	template <typename _Iter>
	struct custom_iterator :
	// public iterator<bidirectional_iterator_tag, _Iter>
	{

		//   typedef _Iter value_type;
		// 	typedef _Iter& reference;
		// 	typedef _Iter* pointer;
			// typedef bidirectional_iterator_tag iterator_category;
		// 	typedef ptrdiff_t difference_type;
	using iterator_category = ft::bidirectional_iterator_tag;
	using value_type        = _Iter;
	using difference_type   = std::ptrdiff_t;
	using reference         = value_type&;
	// using pointer           = value_type*;
		public:
			custom_iterator() {}
	};
};

// void test1(ft::bidirectional_iterator_tag, )

struct Iter {
  using iterator_category = std::random_access_iterator_tag;
  using value_type        = int;
  using difference_type   = std::ptrdiff_t;
  using reference         = value_type&;
  using pointer           = value_type*;
};



#include <vector>
#include <cassert>
#include <type_traits>

int main()
{
	ft::custom_iterator<int> iter;
	std::vector<int> v;
	// static_assert(std::is_same<std::iterator_traits<Iter>::iterator_category, 
    //                          std::random_access_iterator_tag>::value);
	//   std::cout << std::__has_iterator_typedefs<ft::custom_iterator<int>::iterator_category>::value << std::endl;  // output '0';
	//   std::cout << std::__has_iterator_typedefs<std::vector<int>::iterator::iterator_category>::value << std::endl;  // output '0';
  // std::cout << (typeid(std::iterator_traits<Iter>::iterator_category) == typeid(std::random_access_iterator_tag)) << std::endl;
//   static_assert(std::is_same<ft::iterator_traits<ft::custom_iterator<int>::iterator_category>,
//   					std::bidirectional_iterator_tag>::value);
  static_assert(std::is_same<ft::iterator_traits<ft::custom_iterator<int> >::iterator_category,
  					ft::bidirectional_iterator_tag>::value);
//   ft::is_same<std::iterator_traits<ft::custom_iterator<int>::iterator_category>, std::bidirectional_iterator_tag>;
	return (0);
}