#ifndef VECTOR_HPP
# define VECTOR_HPP

#include <iterator>

namespace ft {

   template<typename T, typename Allocator = std::allocator<T>>
   class vector {
      public:
         typedef T value_type;
         typedef Allocator allocator_type;
         typedef std::size_t size_type;
         typedef std::ptrdiff_t difference_type;
         typedef value_type& reference;
         typedef const value_type& const_reference;
         typedef Allocator::pointer pointer;
         typedef const Allocator::pointer const_pointer;
         typedef RandomAccessIterator<value_type> iterator;
      private:
         pointer        _first;
         size_type      _size, _capacity;
         allocator_type _allocator;

   };
};

#endif