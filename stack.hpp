#ifndef STACK_HPP
# define STACK_HPP

#include "vector.hpp"

namespace ft {

template <class _Tp, class _Container = vector<_Tp> >
class stack
{
public:
    typedef _Container                               container_type;
    typedef typename container_type::value_type      value_type;
    typedef typename container_type::reference       reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::size_type       size_type;

protected:
    container_type c;

public:
    stack()
        : c() {}

    stack(const stack& __q) : c(__q.c) {}

    stack& operator=(const stack& __q) {c = __q.c; return *this;}

    explicit stack(const container_type& __c) : c(__c) {}

    template <class _Alloc>
        explicit stack(const _Alloc& __a)
            : c(__a) {}
    template <class _Alloc>
        stack(const container_type& __c, const _Alloc& __a)
            : c(__c, __a) {}
    template <class _Alloc>
        stack(const stack& __s, const _Alloc& __a)
            : c(__s.c, __a) {}

    bool empty()     const      {return c.empty();}

    size_type size() const      {return c.size();}

    reference top()             {return c.back();}

    const_reference top() const {return c.back();}

    void push(const value_type& __v) {c.push_back(__v);}

    void pop() {c.pop_back();}

    template <class T1, class _C1>
    friend
    bool
    operator==(const stack<T1, _C1>& __x, const stack<T1, _C1>& __y);

    template <class T1, class _C1>
    friend
    bool
    operator< (const stack<T1, _C1>& __x, const stack<T1, _C1>& __y);
};

template <class _Tp, class _Container>
bool
operator==(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
{
    return __x.c == __y.c;
}

template <class _Tp, class _Container>
bool
operator< (const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
{
    return __x.c < __y.c;
}

template <class _Tp, class _Container>
bool
operator!=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
{
    return !(__x == __y);
}

template <class _Tp, class _Container>
bool
operator> (const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
{
    return __y < __x;
}

template <class _Tp, class _Container>
bool
operator>=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
{
    return !(__x < __y);
}

template <class _Tp, class _Container>
bool
operator<=(const stack<_Tp, _Container>& __x, const stack<_Tp, _Container>& __y)
{
    return !(__y < __x);
}

};

#endif