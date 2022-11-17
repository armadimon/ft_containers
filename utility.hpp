#ifndef UTILITY_HPP
# define UTILITY_HPP

#define NOEXCEPT throw()

namespace ft {


    template <bool, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> {
        typedef T type;
    };

    template<class T, T v>
    struct integral_constant {
        typedef T value_type;
        static const T value = v;
        operator value_type() const NOEXCEPT { return value; }
    };

    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;


	template <typename T, typename U>
	struct is_same : public false_type {};

	template <typename T>
	struct is_same<T, T> : public true_type {};

    template <class _Tp> struct is_integral                     : public false_type {};
    template <>          struct is_integral<bool>               : public true_type {};
    template <>          struct is_integral<char>               : public true_type {};
    template <>          struct is_integral<signed char>        : public true_type {};
    template <>          struct is_integral<unsigned char>      : public true_type {};
    template <>          struct is_integral<wchar_t>            : public true_type {};
    template <>          struct is_integral<short>              : public true_type {};
    template <>          struct is_integral<unsigned short>     : public true_type {};
    template <>          struct is_integral<int>                : public true_type {};
    template <>          struct is_integral<unsigned int>       : public true_type {};
    template <>          struct is_integral<long>               : public true_type {};
    template <>          struct is_integral<unsigned long>      : public true_type {};
    template <>          struct is_integral<long long>          : public true_type {};
    template <>          struct is_integral<unsigned long long> : public true_type {};
    template <>          struct is_integral<char16_t>           : public true_type {};

    template <typename _T1, typename _T2>
    struct pair
    {
        typedef _T1 first_type;
        typedef _T2 second_type;

        _T1 first;
        _T2 second;

        // pair(pair const&) {}
        // explicit pair(_T1 const& __t1, _T2 const& __t2) : first(__t1), second(__t2) {}

        pair() : first(), second() {}
        template <typename _U1, typename _U2>
        pair(const pair<_U1, _U2> &other)
            : first(other.first), second(other.second) {}
        pair(const _T1 &t1, const _T2 &t2) : first(t1), second(t2) {}

        // assign operator
        pair &operator=(pair const &other) {
            first = other.first;
            second = other.second;
            return *this;
        }
    };
    

    template <class T1,class T2>
    pair<T1,T2> make_pair (T1 x, T2 y)
    {
        return ( pair<T1,T2>(x,y) );
    }

    template<class InputIt1, class InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1, 
            InputIt2 first2)
    {
        for (; first1 != last1; ++first1, ++first2) {
            if (!(*first1 == *first2)) {
                return false;
            }
        }
        return true;
    }

    template<class InputIt1, class InputIt2, class BinaryPredicate>
    bool equal(InputIt1 first1, InputIt1 last1, 
            InputIt2 first2, BinaryPredicate p)
    {
        for (; first1 != last1; ++first1, ++first2) {
            if (!p(*first1, *first2)) {
                return false;
            }
        }
        return true;
    }

    template<class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                InputIt2 first2, InputIt2 last2)
    {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
        {
            if (*first1 < *first2)
                return true;
            if (*first2 < *first1)
                return false;
        }
    
        return (first1 == last1) && (first2 != last2);
    }

    template<class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                InputIt2 first2, InputIt2 last2, Compare comp)
    {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
        {
            if (comp(*first1, *first2))
                return true;
            if (comp(*first2, *first1))
                return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template <typename T>
    const T& min(const T& x, const T& y) {
        return (x < y) ? x : y;
}
template <class _Arg, class _Result>
     struct unary_function
     {
       typedef _Arg argument_type;   
 
 
       typedef _Result result_type;  
     };
 

template <class _Pair>
     struct _Select1st : unary_function<_Pair,
                           typename _Pair::first_type>
     {
       typename _Pair::first_type&
       operator()(_Pair& __x) const
       { return __x.first; }
 
       const typename _Pair::first_type&
       operator()(const _Pair& __x) const
       { return __x.first; }
     };

};

#endif