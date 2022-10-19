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

};


#endif