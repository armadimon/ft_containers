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
};


#endif