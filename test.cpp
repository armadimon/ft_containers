
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

#include <cstddef>
#include <iterator>
#include <iostream>

#include "vector.hpp"

// struct Add{
// 	Add(int toAdd) : _toAdd(toAdd) {} // default constructor
// 	int operator()(int x){
// 		return x + _toAdd;
// 	}
// 	private:
// 	int _toAdd;
// };

// // int main(){
// // 	Add add_42(42); // state 저장
// // 	Add add_13(13);
// // 	int result_42 = add_42(2);
// //         std::cout << result_42 << " ";
// // 	int result_13 = add_13(2);
// //         std::cout << result_13 << " ";
// // 	std::vector<int> v(10, 10);   
// //     for (std::vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
// //         std::cout << *iter << " ";
// //     }
// // 	std::transform(v.begin(), v.end(), v.begin(), add_42);
// //     for (std::vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
// //         std::cout << *iter << " ";
// //     }

// // 	std::vector<int> v2(10, 10);

// // 	std::vector<int>::iterator iter1 = v2.begin();
// // 	std::vector<int>::iterator iter2 = v2.end();

// // 	std::vector<int> v3(2, 10);

// // 	v3.assign(iter1, iter2); // assign에서 range를 사용하는 문법은 assign(inputiter 1, inputiter 2)이고, 여기서는 pointer로 특수화된 vector의 iter가 random access iter tag를 가지고 있기 때문에 iterator의 상속 구조에 따라서 inputiterator로도 동작 할 수 있는 것이다.
// // 	for (std::vector<int>::iterator iter = v3.begin(); iter != v3.end(); iter++) {
// // 		std::cout << *iter << " ";
// //     }
// // }

// namespace ft
// {

// struct nothing {};
// struct input_iterator_tag {};
// struct output_iterator_tag {};
// struct forward_iterator_tag : input_iterator_tag {};
// struct bidirectional_iterator_tag : forward_iterator_tag {};
// struct random_access_iterator_tag : bidirectional_iterator_tag {};

// template <typename _Category, typename _T, typename _Distance = ptrdiff_t,
//           typename _pointer = _T *, typename _reference = _T &>
// struct iterator {
//   typedef _T value_type;
//   typedef _Distance difference_type;
//   typedef _pointer pointer;
//   typedef _reference reference;
//   typedef _Category iterator_category;
// };

// 	template <typename>
// 	struct void_t {
// 	typedef void type;
// 	};

// 	template <typename _T>
// 	struct __has_iterator_typedefs {
// 	private:
// 	struct __two {
// 		char __x;
// 		char __y;
// 	};
// 	template <typename _U>
// 	static __two __test(...){};
// 	template <typename _U>
// 	static char __test(
// 		typename ft::void_t<typename _U::iterator_category>::type * = 0,
// 		typename ft::void_t<typename _U::difference_type>::type * = 0,
// 		typename ft::void_t<typename _U::value_type>::type * = 0,
// 		typename ft::void_t<typename _U::reference>::type * = 0,
// 		typename ft::void_t<typename _U::pointer>::type * = 0){};

// 	public:
// 	static const bool value = sizeof(__test<_T>(0, 0, 0, 0, 0)) == 1;
// 	};

// 	template <typename _Iter, bool>
// 	struct __iterator_traits_typedefs {};

// 	template <typename _Iter>
// 	struct __iterator_traits_typedefs<_Iter, true> {
// 	typedef typename _Iter::iterator_category iterator_category;
// 	typedef typename _Iter::value_type value_type;
// 	typedef typename _Iter::difference_type difference_type;
// 	typedef typename _Iter::pointer pointer;
// 	typedef typename _Iter::reference reference;
// 	};

// 	template <typename _Iter, bool>
// 	struct __iterator_traits {};




// template <typename T, typename U>
// struct is_same : public std::false_type {};

// template <typename T>
// struct is_same<T, T> : public std::true_type {};

// 	template <typename _Iter>
// 	struct __iterator_traits<_Iter, true>
// 		: __iterator_traits_typedefs<
// 			_Iter, is_same<typename _Iter::iterator_category,
// 							input_iterator_tag>::value ||
// 						is_same<typename _Iter::iterator_category,
// 								output_iterator_tag>::value ||
// 						is_same<typename _Iter::iterator_category,
// 								forward_iterator_tag>::value ||
// 						is_same<typename _Iter::iterator_category,
// 								bidirectional_iterator_tag>::value ||
// 						is_same<typename _Iter::iterator_category,
// 								random_access_iterator_tag>::value> {};

// 	template <typename _Iter>
// 	struct iterator_traits
// 		: __iterator_traits<_Iter, __has_iterator_typedefs<_Iter>::value> {};


// 	template <typename _Iter>
// 	struct custom_iterator :
// 	// public iterator<bidirectional_iterator_tag, _Iter>
// 	{

// 		//   typedef _Iter value_type;
// 		// 	typedef _Iter& reference;
// 		// 	typedef _Iter* pointer;
// 			// typedef bidirectional_iterator_tag iterator_category;
// 		// 	typedef ptrdiff_t difference_type;
// 	using iterator_category = ft::bidirectional_iterator_tag;
// 	using value_type        = _Iter;
// 	using difference_type   = std::ptrdiff_t;
// 	using reference         = value_type&;
// 	// using pointer           = value_type*;
// 		public:
// 			custom_iterator() {}
// 	};
// };

// // void test1(ft::bidirectional_iterator_tag, )

// struct Iter {
//   using iterator_category = std::random_access_iterator_tag;
//   using value_type        = int;
//   using difference_type   = std::ptrdiff_t;
//   using reference         = value_type&;
//   using pointer           = value_type*;
// };



// #include <vector>
// #include <cassert>
// #include <type_traits>

// int main()
// {
// 	ft::custom_iterator<int> iter;
// 	std::vector<int> v;
// 	// static_assert(std::is_same<std::iterator_traits<Iter>::iterator_category, 
//     //                          std::random_access_iterator_tag>::value);
// 	//   std::cout << std::__has_iterator_typedefs<ft::custom_iterator<int>::iterator_category>::value << std::endl;  // output '0';
// 	//   std::cout << std::__has_iterator_typedefs<std::vector<int>::iterator::iterator_category>::value << std::endl;  // output '0';
//   // std::cout << (typeid(std::iterator_traits<Iter>::iterator_category) == typeid(std::random_access_iterator_tag)) << std::endl;
// //   static_assert(std::is_same<ft::iterator_traits<ft::custom_iterator<int>::iterator_category>,
// //   					std::bidirectional_iterator_tag>::value);
//   static_assert(std::is_same<ft::iterator_traits<ft::custom_iterator<int> >::iterator_category,
//   					ft::bidirectional_iterator_tag>::value);
// //   ft::is_same<std::iterator_traits<ft::custom_iterator<int>::iterator_category>, std::bidirectional_iterator_tag>;
// 	return (0);
// }

void ft_test()
{
	// ft::vector<int> a(3);
	// ft::vector<int>::iterator _iter = a.begin();
	// std::cout << *_iter << std::endl;
	// std::cout << a.capacity() << std::endl;
	// std::cout << a.size() << std::endl;
	// std::cout << a[0] << std::endl;
	// a.reserve(50);
	// a.push_back(1);
	// a.push_back(2);
	// a.push_back(3);
	// 	std::cout << *_iter << std::endl;
	// std::cout << a.capacity() << std::endl;
	// std::cout << a.size() << std::endl;
	// std::cout << a[0] << std::endl;

	ft::vector<int> a(10);
	// a.push_back(1);
	// a.push_back(2);
	// a.push_back(3);
	// a.push_back(4);
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	a[3] = 4;
	// for (int i = 0; i < 500; i++)
	// 	a.push_back(1);

	// a.at(3) = 3;
	ft::vector<int>::iterator _iter = a.begin();
	ft::iterator_traits<std::vector<int>::iterator>::value_type;
	// ft::reverse_iterator<std::vector<int>::iterator> my_rit(a.end()), my_rit1(a.end() - 1);
	std::cout << _iter[0] << std::endl;
	std::cout << _iter[1] << std::endl;
	std::cout << _iter[2] << std::endl;
	std::cout << _iter[3] << std::endl;
	std::cout << _iter[4] << std::endl;
	std::cout << _iter[5] << std::endl;
	std::cout << _iter[6] << std::endl;
	std::cout << a.capacity() << std::endl;
	std::cout << a.size() << std::endl;
	// std::cout << *a.begin() << std::endl;
	// std::cout << *a.end() << std::endl;
	ft::vector<int> a2;
	// for (int i = 0; i < 500; i++)
	// 	a2.push_back(1);
	a2.push_back(1);
	a2.push_back(2);
	a2.push_back(3);
	a2.push_back(4);
	ft::vector<int>::iterator _iter2 = a2.begin();
	std::cout << _iter2[0] << std::endl;
	std::cout << _iter2[1] << std::endl;
	std::cout << _iter2[2] << std::endl;
	std::cout << a2.capacity() << std::endl;
	std::cout << a2.size() << std::endl;
	// std::cout << *a2.begin() << std::endl;
	// std::cout << *a2.end() << std::endl;
	ft::vector<int> my_v(3, 4);
	ft::vector<int> v4(a2.begin(), a2.end());
	ft::vector<int>::iterator _iter4 = v4.begin();
	std::cout << _iter4[0] << std::endl;
	std::cout << _iter4[1] << std::endl;
	std::cout << _iter4[2] << std::endl;
	std::cout << v4.capacity() << std::endl;
	std::cout << v4.size() << std::endl;
	std::cout << std::__has_iterator_typedefs<std::vector<int>::iterator>::value << std::endl;
	std::cout << std::__has_iterator_typedefs<ft::vector<int>::iterator>::value << std::endl;
	ft::vector<char> v5;
	v5.push_back('a');
	v5.push_back('b');
	v5.push_back('c');
	std::cout << v4.capacity() << std::endl;
	std::cout << v4.size() << std::endl;
}

template<class Os, class Co> Os& operator<<(Os& os, const Co& co) {
    os << "{";
    for (auto const& i : co) { os << ' ' << i; }
    return os << " } ";
}
 
void test_swap()
{
    ft::vector<int> a1, a2;

	a1.push_back(1);
	a1.push_back(2);
	a1.push_back(3);

	a2.push_back(4);
	a2.push_back(5);
 
    auto it1 = a1.begin();
    auto it2 = a2.begin();
 
    int& ref1 = a1.front();
    int& ref2 = a2.front();
 
    std::cout << a1 << a2 << '\n';
    a1.swap(a2);
    std::cout << a1 << a2 << '\n';
 
    // Note that after swap the iterators and references stay associated with their
    // original elements, e.g. it1 that pointed to an element in 'a1' with value 2
    // still points to the same element, though this element was moved into 'a2'.
}


	void print(int id, const std::vector<int>& container)
{
    std::cout << id << ". ";
    for (const int x: container)
        std::cout << x << ' ';
    std::cout << '\n';
}
 	void print(int id, const ft::vector<int>& container)
{
    std::cout << id << ". ";
    for (const int x: container)
        std::cout << x << ' ';
    std::cout << '\n';
}
 
void test_insert()
{
    // ft::vector<int> c1(5, 100);
    std::vector<int> c1;

	c1.push_back(1);
	c1.push_back(2);
	c1.push_back(3);
	c1.push_back(4);
	c1.push_back(5);
	// c1.reserve(100);
	// std::cout << c1.capacity() << "\n";
    print(1, c1);
 
    auto it = c1.begin();
    it = c1.insert(it, 200);
	std::cout << *it << "it \n";
	// std::cout << c1.capacity() << "\n";
    print(2, c1);
 
    c1.insert(it, 2, 300);
    print(3, c1);
 
    // `it` no longer valid, get a new one:
    it = c1.begin();
 
    std::vector<int> c2(2, 400);
    // c1.insert(std::next(it, 2), c2.begin(), c2.end());
    c1.insert(it + 2, c2.begin(), c2.end());
    print(4, c1);
 
    // int arr[] = {501, 502, 503};
    // c1.insert(c1.begin(), arr, arr + std::size(arr));
    // print(5, c1);
 
    // c1.insert(c1.end(), {601, 602, 603});
    // print(6, c1);
}

void print_container(const std::vector<int>& c) 
{
    for (int i : c)
        std::cout << i << " ";
    std::cout << '\n';
}

void print_container(const ft::vector<int>& c) 
{
    for (int i : c)
        std::cout << i << " ";
    std::cout << '\n';
}

void test_erase()
{
	
	std::vector<int> c;
	for (int i = 1; i < 10; i++)
		c.push_back(i);
    print_container(c);

	std::cout << c.size() << std::endl;
    c.erase(c.begin());
    print_container(c);

	std::cout << c.size() << std::endl;
 
    c.erase(c.begin()+2, c.begin()+5);
    print_container(c);
 
    // Erase all even numbers
    for (std::vector<int>::iterator it = c.begin(); it != c.end();)
    {
        if (*it % 2 == 0)
            it = c.erase(it);
        else
            ++it;
    }
    print_container(c);
}

void test_nomal()
{
	 std::string str, ft_str;
        /*
         * var to store the size and the capacity
         */
        ft::vector<std::string>::size_type s, ft_s;
        ft::vector<std::string>::size_type c, ft_c;
        std::vector<std::string>::iterator it;
        ft::vector<std::string>::iterator ft_it;
        /*
         * bool to store the comparison
         */
        bool cond;
        /*------------------ test 1: erase from the begin to end -------------------*/
        {
            std::vector<std::string> v(100, "hello");
            ft::vector<std::string> ft_v(100, "hello");

            it = v.erase(v.begin(), v.end());
            ft_it = ft_v.erase(ft_v.begin(), ft_v.end());

            s = v.size();
            ft_s = ft_v.size();
            c = v.size();
            ft_c = ft_v.size();
            for (size_t i = 0; i < v.size(); ++i)
                str += v[i];
            for (size_t i = 0; i < ft_v.size(); ++i)
                ft_str += ft_v[i];
            cond = ((str == ft_str) && (s == ft_s) && (c == ft_c));
            // cond = (cond && (std::distance(v.begin(), it) == std::distance(ft_v.begin(), ft_it)));
			std::cout << cond << std::endl;
        }
}

void test_trait()
{
}

template <typename T>
std::vector<int> constructor_test(std::vector<T> vector) {
    std::vector<int> v;
    std::vector<int> tmp0(vector);
    std::vector<int> tmp(1000, 4), tmp2(1000, 5);
    tmp = tmp2;
    std::vector<int> tmp3(tmp);
    std::vector<int> tmp4(tmp.begin(), tmp.end());
    v.push_back(tmp4.size());
    v.push_back(tmp4.capacity());
    v.push_back(tmp[2]);
    v.push_back(tmp3[2]);
    v.push_back(tmp4[2]);
    try { std::vector<int> tmp5(-1, -1); }
    catch (std::exception &e) {
		v.push_back(1);
		std::cout << "check" << std::endl;
		}
    return v;
}

template <typename T>
ft::vector<int> constructor_test(ft::vector<T> vector) {
    ft::vector<int> v;
    ft::vector<int> tmp0(vector);
    ft::vector<int> tmp(1000, 4), tmp2(1000, 5);
    // tmp = tmp2;
    ft::vector<int> tmp3(tmp);
    ft::vector<int> tmp4(tmp.begin(), tmp.end());
    v.push_back(tmp4.size());
    v.push_back(tmp4.capacity());
    v.push_back(tmp[2]);
    v.push_back(tmp3[2]);
    v.push_back(tmp4[2]);
    try { ft::vector<int> tmp5(-1, -1); }
    catch (std::exception &e) {
		v.push_back(1);
		std::cout << "check" << std::endl;
		}
    return v;
}

void test_size()
{
    ft::vector<int> c;
	c.push_back(1);
	c.push_back(2);
	c.push_back(3);
    std::cout << "The vector holds: ";
    for (const auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
 
    c.resize(5);
    std::cout << "After resize up to 5: ";
    for (const auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
 
    c.resize(2);
    std::cout << "After resize down to 2: ";
    for (const auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
 
    c.resize(6, 4);
    std::cout << "After resize up to 6 (initializer = 4): ";
    for (const auto& el: c) std::cout << el << ' ';
    std::cout << '\n';
}

int main()
{
	// ft_test();
	// test_swap();
	// test_insert();
	// test_erase();
	// test_nomal();
	// ft::vector<int> v1;
	// std::vector<int> v2;
	// begin_test(v1);
	// begin_test(v2);
	test_size();
	// system("leaks a.out");
	return (0);
}
// template <class Tp>
// struct NAlloc {
//     typedef Tp value_type;
//     NAlloc() = default;
//     template <class T> NAlloc(const NAlloc<T>&) {}
 
//     Tp* allocate(std::size_t n)
//     {
//         n *= sizeof(Tp);
//         Tp* p = static_cast<Tp*>(::operator new(n));
//         std::cout << "allocating " << n << " bytes @ " << p << '\n';
//         return p;
//     }
 
//     void deallocate(Tp* p, std::size_t n)
//     {
//         std::cout << "deallocating " << n*sizeof*p << " bytes @ " << p << "\n\n";
//         ::operator delete(p);
//     }
// };
// template <class T, class U>
// bool operator==(const NAlloc<T>&, const NAlloc<U>&) { return true; }
// template <class T, class U>
// bool operator!=(const NAlloc<T>&, const NAlloc<U>&) { return false; }
 
// int main()
// {
//     constexpr int max_elements = 32;
 
//     std::cout << "using reserve: \n";
//     {
//         ft::vector<int> v1;
//         v1.reserve( max_elements ); // reserves at least max_elements * sizeof(int) bytes
 
//         for(int n = 0; n < max_elements; ++n)
//             ;
//     }
 
//     std::cout << "not using reserve: \n";
//     {
//         std::vector<int, NAlloc<int> > v1;
 
//         for(int n = 0; n < max_elements; ++n) {
//             if(v1.size() == v1.capacity()) {
//                 std::cout << "size() == capacity() == " << v1.size() << '\n';
//             }
//             v1.push_back(n);
//         }
//     }
// }