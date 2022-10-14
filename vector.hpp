
   #ifndef _STL_VECTOR_H
   #define _STL_VECTOR_H 1
   58 
   59#include <bits/stl_iterator_base_funcs.h>
   60#include <bits/functexcept.h>
   61#include <bits/concept_check.h>
   62#if __cplusplus >= 201103L
   63#include <initializer_list>
   64#endif
   65#if __cplusplus > 201703L
   66# include <compare>
   67#endif
   68 
   69#include <debug/assertions.h>
   70 
   71#if _GLIBCXX_SANITIZE_STD_ALLOCATOR && _GLIBCXX_SANITIZE_VECTOR
   72extern "C" void
   73__sanitizer_annotate_contiguous_container(const void*, const void*,
   74                                          const void*, const void*);
   75#endif
   76 
   77namespace std _GLIBCXX_VISIBILITY(default)
   78{
   79_GLIBCXX_BEGIN_NAMESPACE_VERSION
   80_GLIBCXX_BEGIN_NAMESPACE_CONTAINER
   81 
   82  /// See bits/stl_deque.h's _Deque_base for an explanation.
   83  template<typename _Tp, typename _Alloc>
   84    struct _Vector_base
   85    {
   86      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
   87        rebind<_Tp>::other _Tp_alloc_type;
   88      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
   89        pointer;
   90 
   91      struct _Vector_impl_data
   92      {
   93        pointer _M_start;
   94        pointer _M_finish;
   95        pointer _M_end_of_storage;
   96 
   97        _Vector_impl_data() _GLIBCXX_NOEXCEPT
   98        : _M_start(), _M_finish(), _M_end_of_storage()
   99        { }
  100 
  101#if __cplusplus >= 201103L
  102        _Vector_impl_data(_Vector_impl_data&& __x) noexcept
  103        : _M_start(__x._M_start), _M_finish(__x._M_finish),
  104          _M_end_of_storage(__x._M_end_of_storage)
  105        { __x._M_start = __x._M_finish = __x._M_end_of_storage = pointer(); }
  106#endif
  107 
  108        void
  109        _M_copy_data(_Vector_impl_data const& __x) _GLIBCXX_NOEXCEPT
  110        {
  111          _M_start = __x._M_start;
  112          _M_finish = __x._M_finish;
  113          _M_end_of_storage = __x._M_end_of_storage;
  114        }
  115 
  116        void
  117        _M_swap_data(_Vector_impl_data& __x) _GLIBCXX_NOEXCEPT
  118        {
  119          // Do not use std::swap(_M_start, __x._M_start), etc as it loses
  120          // information used by TBAA.
  121          _Vector_impl_data __tmp;
  122          __tmp._M_copy_data(*this);
  123          _M_copy_data(__x);
  124          __x._M_copy_data(__tmp);
  125        }
  126      };
  127 
  128      struct _Vector_impl
  129        : public _Tp_alloc_type, public _Vector_impl_data
  130      {
  131        _Vector_impl() _GLIBCXX_NOEXCEPT_IF(
  132            is_nothrow_default_constructible<_Tp_alloc_type>::value)
  133        : _Tp_alloc_type()
  134        { }
  135 
  136        _Vector_impl(_Tp_alloc_type const& __a) _GLIBCXX_NOEXCEPT
  137        : _Tp_alloc_type(__a)
  138        { }
  139 
  140#if __cplusplus >= 201103L
  141        // Not defaulted, to enforce noexcept(true) even when
  142        // !is_nothrow_move_constructible<_Tp_alloc_type>.
  143        _Vector_impl(_Vector_impl&& __x) noexcept
  144        : _Tp_alloc_type(std::move(__x)), _Vector_impl_data(std::move(__x))
  145        { }
  146 
  147        _Vector_impl(_Tp_alloc_type&& __a) noexcept
  148        : _Tp_alloc_type(std::move(__a))
  149        { }
  150 
  151        _Vector_impl(_Tp_alloc_type&& __a, _Vector_impl&& __rv) noexcept
  152        : _Tp_alloc_type(std::move(__a)), _Vector_impl_data(std::move(__rv))
  153        { }
  154#endif
  155 
  156#if _GLIBCXX_SANITIZE_STD_ALLOCATOR && _GLIBCXX_SANITIZE_VECTOR
  157        template<typename = _Tp_alloc_type>
  158          struct _Asan
  159          {
  160            typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>
  161              ::size_type size_type;
  162 
  163            static void _S_shrink(_Vector_impl&, size_type) { }
  164            static void _S_on_dealloc(_Vector_impl&) { }
  165 
  166            typedef _Vector_impl& _Reinit;
  167 
  168            struct _Grow
  169            {
  170              _Grow(_Vector_impl&, size_type) { }
  171              void _M_grew(size_type) { }
  172            };
  173          };
  174 
  175        // Enable ASan annotations for memory obtained from std::allocator.
  176        template<typename _Up>
  177          struct _Asan<allocator<_Up> >
  178          {
  179            typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>
  180              ::size_type size_type;
  181 
  182            // Adjust ASan annotation for [_M_start, _M_end_of_storage) to
  183            // mark end of valid region as __curr instead of __prev.
  184            static void
  185            _S_adjust(_Vector_impl& __impl, pointer __prev, pointer __curr)
  186            {
  187              __sanitizer_annotate_contiguous_container(__impl._M_start,
  188                  __impl._M_end_of_storage, __prev, __curr);
  189            }
  190 
  191            static void
  192            _S_grow(_Vector_impl& __impl, size_type __n)
  193            { _S_adjust(__impl, __impl._M_finish, __impl._M_finish + __n); }
  194 
  195            static void
  196            _S_shrink(_Vector_impl& __impl, size_type __n)
  197            { _S_adjust(__impl, __impl._M_finish + __n, __impl._M_finish); }
  198 
  199            static void
  200            _S_on_dealloc(_Vector_impl& __impl)
  201            {
  202              if (__impl._M_start)
  203                _S_adjust(__impl, __impl._M_finish, __impl._M_end_of_storage);
  204            }
  205 
  206            // Used on reallocation to tell ASan unused capacity is invalid.
  207            struct _Reinit
  208            {
  209              explicit _Reinit(_Vector_impl& __impl) : _M_impl(__impl)
  210              {
  211                // Mark unused capacity as valid again before deallocating it.
  212                _S_on_dealloc(_M_impl);
  213              }
  214 
  215              ~~_Reinit()
  216              {
  217                // Mark unused capacity as invalid after reallocation.
  218                if (_M_impl._M_start)
  219                  _S_adjust(_M_impl, _M_impl._M_end_of_storage,
  220                            _M_impl._M_finish);
  221              }
  222 
  223              _Vector_impl& _M_impl;
  224 
  225#if __cplusplus >= 201103L
  226              _Reinit(const _Reinit&) = delete;
  227              _Reinit& operator=(const _Reinit&) = delete;
  228#endif
  229            };
  230 
  231            // Tell ASan when unused capacity is initialized to be valid.
  232            struct _Grow
  233            {
  234              _Grow(_Vector_impl& __impl, size_type __n)
  235              : _M_impl(__impl), _M_n(__n)
  236              { _S_grow(_M_impl, __n); }
  237 
  238              ~~_Grow() { if (_M_n) _S_shrink(_M_impl, _M_n); }
  239 
  240              void _M_grew(size_type __n) { _M_n -= __n; }
  241 
  242#if __cplusplus >= 201103L
  243              _Grow(const _Grow&) = delete;
  244              _Grow& operator=(const _Grow&) = delete;
  245#endif
  246            private:
  247              _Vector_impl& _M_impl;
  248              size_type _M_n;
  249            };
  250          };
  251 
  252#define _GLIBCXX_ASAN_ANNOTATE_REINIT \
  253  typename _Base::_Vector_impl::template _Asan<>::_Reinit const \
  254        __attribute__((__unused__)) __reinit_guard(this->_M_impl)
  255#define _GLIBCXX_ASAN_ANNOTATE_GROW(n) \
  256  typename _Base::_Vector_impl::template _Asan<>::_Grow \
  257        __attribute__((__unused__)) __grow_guard(this->_M_impl, (n))
  258#define _GLIBCXX_ASAN_ANNOTATE_GREW(n) __grow_guard._M_grew(n)
  259#define _GLIBCXX_ASAN_ANNOTATE_SHRINK(n) \
  260  _Base::_Vector_impl::template _Asan<>::_S_shrink(this->_M_impl, n)
  261#define _GLIBCXX_ASAN_ANNOTATE_BEFORE_DEALLOC \
  262  _Base::_Vector_impl::template _Asan<>::_S_on_dealloc(this->_M_impl)
  263#else // ! (_GLIBCXX_SANITIZE_STD_ALLOCATOR && _GLIBCXX_SANITIZE_VECTOR)
  264#define _GLIBCXX_ASAN_ANNOTATE_REINIT
  265#define _GLIBCXX_ASAN_ANNOTATE_GROW(n)
  266#define _GLIBCXX_ASAN_ANNOTATE_GREW(n)
  267#define _GLIBCXX_ASAN_ANNOTATE_SHRINK(n)
  268#define _GLIBCXX_ASAN_ANNOTATE_BEFORE_DEALLOC
  269#endif // _GLIBCXX_SANITIZE_STD_ALLOCATOR && _GLIBCXX_SANITIZE_VECTOR
  270      };
  271 
  272    public:
  273      typedef _Alloc allocator_type;
  274 
  275      _Tp_alloc_type&
  276      _M_get_Tp_allocator() _GLIBCXX_NOEXCEPT
  277      { return this->_M_impl; }
  278 
  279      const _Tp_alloc_type&
  280      _M_get_Tp_allocator() const _GLIBCXX_NOEXCEPT
  281      { return this->_M_impl; }
  282 
  283      allocator_type
  284      get_allocator() const _GLIBCXX_NOEXCEPT
  285      { return allocator_type(_M_get_Tp_allocator()); }
  286 
  287#if __cplusplus >= 201103L
  288      _Vector_base() = default;
  289#else
  290      _Vector_base() { }
  291#endif
  292 
  293      _Vector_base(const allocator_type& __a) _GLIBCXX_NOEXCEPT
  294      : _M_impl(__a) { }
  295 
  296      // Kept for ABI compatibility.
  297#if !_GLIBCXX_INLINE_VERSION
  298      _Vector_base(size_t __n)
  299      : _M_impl()
  300      { _M_create_storage(__n); }
  301#endif
  302 
  303      _Vector_base(size_t __n, const allocator_type& __a)
  304      : _M_impl(__a)
  305      { _M_create_storage(__n); }
  306 
  307#if __cplusplus >= 201103L
  308      _Vector_base(_Vector_base&&) = default;
  309 
  310      // Kept for ABI compatibility.
  311# if !_GLIBCXX_INLINE_VERSION
  312      _Vector_base(_Tp_alloc_type&& __a) noexcept
  313      : _M_impl(std::move(__a)) { }
  314 
  315      _Vector_base(_Vector_base&& __x, const allocator_type& __a)
  316      : _M_impl(__a)
  317      {
  318        if (__x.get_allocator() == __a)
  319          this->_M_impl._M_swap_data(__x._M_impl);
  320        else
  321          {
  322            size_t __n = __x._M_impl._M_finish - __x._M_impl._M_start;
  323            _M_create_storage(__n);
  324          }
  325      }
  326# endif
  327 
  328      _Vector_base(const allocator_type& __a, _Vector_base&& __x)
  329      : _M_impl(_Tp_alloc_type(__a), std::move(__x._M_impl))
  330      { }
  331#endif
  332 
  333      ~~_Vector_base() _GLIBCXX_NOEXCEPT
  334      {
  335        _M_deallocate(_M_impl._M_start,
  336                      _M_impl._M_end_of_storage - _M_impl._M_start);
  337      }
  338 
  339    public:
  340      _Vector_impl _M_impl;
  341 
  342      pointer
  343      _M_allocate(size_t __n)
  344      {
  345        typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
  346        return __n != 0 ? _Tr::allocate(_M_impl, __n) : pointer();
  347      }
  348 
  349      void
  350      _M_deallocate(pointer __p, size_t __n)
  351      {
  352        typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Tr;
  353        if (__p)
  354          _Tr::deallocate(_M_impl, __p, __n);
  355      }
  356 
  357    protected:
  358      void
  359      _M_create_storage(size_t __n)
  360      {
  361        this->_M_impl._M_start = this->_M_allocate(__n);
  362        this->_M_impl._M_finish = this->_M_impl._M_start;
  363        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
  364      }
  365    };
  366 
  367  /**
  368   *  @brief A standard container which offers fixed time access to
  369   *  individual elements in any order.
  370   *
  371   *  @ingroup sequences
  372   *
  373   *  @tparam _Tp  Type of element.
  374   *  @tparam _Alloc  Allocator type, defaults to allocator<_Tp>.
  375   *
  376   *  Meets the requirements of a <a href="tables.html#65">container</a>, a
  377   *  <a href="tables.html#66">reversible container</a>, and a
  378   *  <a href="tables.html#67">sequence</a>, including the
  379   *  <a href="tables.html#68">optional sequence requirements</a> with the
  380   *  %exception of @c push_front and @c pop_front.
  381   *
  382   *  In some terminology a %vector can be described as a dynamic
  383   *  C-style array, it offers fast and efficient access to individual
  384   *  elements in any order and saves the user from worrying about
  385   *  memory and size allocation.  Subscripting ( @c [] ) access is
  386   *  also provided as with C-style arrays.
  387  */
  388  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
  389    class vector : protected _Vector_base<_Tp, _Alloc>
  390    {
  391#ifdef _GLIBCXX_CONCEPT_CHECKS
  392      // Concept requirements.
  393      typedef typename _Alloc::value_type               _Alloc_value_type;
  394# if __cplusplus < 201103L
  395      __glibcxx_class_requires(_Tp, _SGIAssignableConcept)
  396# endif
  397      __glibcxx_class_requires2(_Tp, _Alloc_value_type, _SameTypeConcept)
  398#endif
  399 
  400#if __cplusplus >= 201103L
  401      static_assert(is_same<typename remove_cv<_Tp>::type, _Tp>::value,
  402          "std::vector must have a non-const, non-volatile value_type");
  403# if __cplusplus > 201703L || defined __STRICT_ANSI__
  404      static_assert(is_same<typename _Alloc::value_type, _Tp>::value,
  405          "std::vector must have the same value_type as its allocator");
  406# endif
  407#endif
  408 
  409      typedef _Vector_base<_Tp, _Alloc>                 _Base;
  410      typedef typename _Base::_Tp_alloc_type            _Tp_alloc_type;
  411      typedef __gnu_cxx::__alloc_traits<_Tp_alloc_type> _Alloc_traits;
  412 
  413    public:
  414      typedef _Tp                                       value_type;
  415      typedef typename _Base::pointer                   pointer;
  416      typedef typename _Alloc_traits::const_pointer     const_pointer;
  417      typedef typename _Alloc_traits::reference         reference;
  418      typedef typename _Alloc_traits::const_reference   const_reference;
  419      typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
  420      typedef __gnu_cxx::__normal_iterator<const_pointer, vector>
  421      const_iterator;
  422      typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;
  423      typedef std::reverse_iterator<iterator>           reverse_iterator;
  424      typedef size_t                                    size_type;
  425      typedef ptrdiff_t                                 difference_type;
  426      typedef _Alloc                                    allocator_type;
  427 
  428    private:
  429#if __cplusplus >= 201103L
  430      static constexpr bool
  431      _S_nothrow_relocate(true_type)
  432      {
  433        return noexcept(std::__relocate_a(std::declval<pointer>(),
  434                                          std::declval<pointer>(),
  435                                          std::declval<pointer>(),
  436                                          std::declval<_Tp_alloc_type&>()));
  437      }
  438 
  439      static constexpr bool
  440      _S_nothrow_relocate(false_type)
  441      { return false; }
  442 
  443      static constexpr bool
  444      _S_use_relocate()
  445      {
  446        // Instantiating std::__relocate_a might cause an error outside the
  447        // immediate context (in __relocate_object_a's noexcept-specifier),
  448        // so only do it if we know the type can be move-inserted into *this.
  449        return _S_nothrow_relocate(__is_move_insertable<_Tp_alloc_type>{});
  450      }
  451 
  452      static pointer
  453      _S_do_relocate(pointer __first, pointer __last, pointer __result,
  454                     _Tp_alloc_type& __alloc, true_type) noexcept
  455      {
  456        return std::__relocate_a(__first, __last, __result, __alloc);
  457      }
  458 
  459      static pointer
  460      _S_do_relocate(pointer, pointer, pointer __result,
  461                     _Tp_alloc_type&, false_type) noexcept
  462      { return __result; }
  463 
  464      static pointer
  465      _S_relocate(pointer __first, pointer __last, pointer __result,
  466                  _Tp_alloc_type& __alloc) noexcept
  467      {
  468        using __do_it = __bool_constant<_S_use_relocate()>;
  469        return _S_do_relocate(__first, __last, __result, __alloc, __do_it{});
  470      }
  471#endif // C++11
  472 
  473    protected:
  474      using _Base::_M_allocate;
  475      using _Base::_M_deallocate;
  476      using _Base::_M_impl;
  477      using _Base::_M_get_Tp_allocator;
  478 
  479    public:
  480      // [23.2.4.1] construct/copy/destroy
  481      // (assign() and get_allocator() are also listed in this section)
  482 
  483      /**
  484       *  @brief  Creates a %vector with no elements.
  485       */
  486#if __cplusplus >= 201103L
  487      vector() = default;
  488#else
  489      vector() { }
  490#endif
  491 
  492      /**
  493       *  @brief  Creates a %vector with no elements.
  494       *  @param  __a  An allocator object.
  495       */
  496      explicit
  497      vector(const allocator_type& __a) _GLIBCXX_NOEXCEPT
  498      : _Base(__a) { }
  499 
  500#if __cplusplus >= 201103L
  501      /**
  502       *  @brief  Creates a %vector with default constructed elements.
  503       *  @param  __n  The number of elements to initially create.
  504       *  @param  __a  An allocator.
  505       *
  506       *  This constructor fills the %vector with @a __n default
  507       *  constructed elements.
  508       */
  509      explicit
  510      vector(size_type __n, const allocator_type& __a = allocator_type())
  511      : _Base(_S_check_init_len(__n, __a), __a)
  512      { _M_default_initialize(__n); }
  513 
  514      /**
  515       *  @brief  Creates a %vector with copies of an exemplar element.
  516       *  @param  __n  The number of elements to initially create.
  517       *  @param  __value  An element to copy.
  518       *  @param  __a  An allocator.
  519       *
  520       *  This constructor fills the %vector with @a __n copies of @a __value.
  521       */
  522      vector(size_type __n, const value_type& __value,
  523             const allocator_type& __a = allocator_type())
  524      : _Base(_S_check_init_len(__n, __a), __a)
  525      { _M_fill_initialize(__n, __value); }
  526#else
  527      /**
  528       *  @brief  Creates a %vector with copies of an exemplar element.
  529       *  @param  __n  The number of elements to initially create.
  530       *  @param  __value  An element to copy.
  531       *  @param  __a  An allocator.
  532       *
  533       *  This constructor fills the %vector with @a __n copies of @a __value.
  534       */
  535      explicit
  536      vector(size_type __n, const value_type& __value = value_type(),
  537             const allocator_type& __a = allocator_type())
  538      : _Base(_S_check_init_len(__n, __a), __a)
  539      { _M_fill_initialize(__n, __value); }
  540#endif
  541 
  542      /**
  543       *  @brief  %Vector copy constructor.
  544       *  @param  __x  A %vector of identical element and allocator types.
  545       *
  546       *  All the elements of @a __x are copied, but any unused capacity in
  547       *  @a __x  will not be copied
  548       *  (i.e. capacity() == size() in the new %vector).
  549       *
  550       *  The newly-created %vector uses a copy of the allocator object used
  551       *  by @a __x (unless the allocator traits dictate a different object).
  552       */
  553      vector(const vector& __x)
  554      : _Base(__x.size(),
  555        _Alloc_traits::_S_select_on_copy(__x._M_get_Tp_allocator()))
  556      {
  557        this->_M_impl._M_finish =
  558          std::__uninitialized_copy_a(__x.begin(), __x.end(),
  559                                      this->_M_impl._M_start,
  560                                      _M_get_Tp_allocator());
  561      }
  562 
  563#if __cplusplus >= 201103L
  564      /**
  565       *  @brief  %Vector move constructor.
  566       *
  567       *  The newly-created %vector contains the exact contents of the
  568       *  moved instance.
  569       *  The contents of the moved instance are a valid, but unspecified
  570       *  %vector.
  571       */
  572      vector(vector&&) noexcept = default;
  573 
  574      /// Copy constructor with alternative allocator
  575      vector(const vector& __x, const allocator_type& __a)
  576      : _Base(__x.size(), __a)
  577      {
  578        this->_M_impl._M_finish =
  579          std::__uninitialized_copy_a(__x.begin(), __x.end(),
  580                                      this->_M_impl._M_start,
  581                                      _M_get_Tp_allocator());
  582      }
  583 
  584    private:
  585      vector(vector&& __rv, const allocator_type& __m, true_type) noexcept
  586      : _Base(__m, std::move(__rv))
  587      { }
  588 
  589      vector(vector&& __rv, const allocator_type& __m, false_type)
  590      : _Base(__m)
  591      {
  592        if (__rv.get_allocator() == __m)
  593          this->_M_impl._M_swap_data(__rv._M_impl);
  594        else if (!__rv.empty())
  595          {
  596            this->_M_create_storage(__rv.size());
  597            this->_M_impl._M_finish =
  598              std::__uninitialized_move_a(__rv.begin(), __rv.end(),
  599                                          this->_M_impl._M_start,
  600                                          _M_get_Tp_allocator());
  601            __rv.clear();
  602          }
  603      }
  604 
  605    public:
  606      /// Move constructor with alternative allocator
  607      vector(vector&& __rv, const allocator_type& __m)
  608      noexcept( noexcept(
  609        vector(std::declval<vector&&>(), std::declval<const allocator_type&>(),
  610               std::declval<typename _Alloc_traits::is_always_equal>())) )
  611      : vector(std::move(__rv), __m, typename _Alloc_traits::is_always_equal{})
  612      { }
  613 
  614      /**
  615       *  @brief  Builds a %vector from an initializer list.
  616       *  @param  __l  An initializer_list.
  617       *  @param  __a  An allocator.
  618       *
  619       *  Create a %vector consisting of copies of the elements in the
  620       *  initializer_list @a __l.
  621       *
  622       *  This will call the element type's copy constructor N times
  623       *  (where N is @a __l.size()) and do no memory reallocation.
  624       */
  625      vector(initializer_list<value_type> __l,
  626             const allocator_type& __a = allocator_type())
  627      : _Base(__a)
  628      {
  629        _M_range_initialize(__l.begin(), __l.end(),
  630                            random_access_iterator_tag());
  631      }
  632#endif
  633 
  634      /**
  635       *  @brief  Builds a %vector from a range.
  636       *  @param  __first  An input iterator.
  637       *  @param  __last  An input iterator.
  638       *  @param  __a  An allocator.
  639       *
  640       *  Create a %vector consisting of copies of the elements from
  641       *  [first,last).
  642       *
  643       *  If the iterators are forward, bidirectional, or
  644       *  random-access, then this will call the elements' copy
  645       *  constructor N times (where N is distance(first,last)) and do
  646       *  no memory reallocation.  But if only input iterators are
  647       *  used, then this will do at most 2N calls to the copy
  648       *  constructor, and logN memory reallocations.
  649       */
  650#if __cplusplus >= 201103L
  651      template<typename _InputIterator,
  652               typename = std::_RequireInputIter<_InputIterator>>
  653        vector(_InputIterator __first, _InputIterator __last,
  654               const allocator_type& __a = allocator_type())
  655        : _Base(__a)
  656        {
  657          _M_range_initialize(__first, __last,
  658                              std::__iterator_category(__first));
  659        }
  660#else
  661      template<typename _InputIterator>
  662        vector(_InputIterator __first, _InputIterator __last,
  663               const allocator_type& __a = allocator_type())
  664        : _Base(__a)
  665        {
  666          // Check whether it's an integral type.  If so, it's not an iterator.
  667          typedef typename std::__is_integer<_InputIterator>::__type _Integral;
  668          _M_initialize_dispatch(__first, __last, _Integral());
  669        }
  670#endif
  671 
  672      /**
  673       *  The dtor only erases the elements, and note that if the
  674       *  elements themselves are pointers, the pointed-to memory is
  675       *  not touched in any way.  Managing the pointer is the user's
  676       *  responsibility.
  677       */
  678      ~vector() _GLIBCXX_NOEXCEPT
  679      {
  680        std::_Destroy(this->_M_impl._M_start, this->_M_impl._M_finish,
  681                      _M_get_Tp_allocator());
  682        _GLIBCXX_ASAN_ANNOTATE_BEFORE_DEALLOC;
  683      }
  684 
  685      /**
  686       *  @brief  %Vector assignment operator.
  687       *  @param  __x  A %vector of identical element and allocator types.
  688       *
  689       *  All the elements of @a __x are copied, but any unused capacity in
  690       *  @a __x will not be copied.
  691       *
  692       *  Whether the allocator is copied depends on the allocator traits.
  693       */
  694      vector&
  695      operator=(const vector& __x);
  696 
  697#if __cplusplus >= 201103L
  698      /**
  699       *  @brief  %Vector move assignment operator.
  700       *  @param  __x  A %vector of identical element and allocator types.
  701       *
  702       *  The contents of @a __x are moved into this %vector (without copying,
  703       *  if the allocators permit it).
  704       *  Afterwards @a __x is a valid, but unspecified %vector.
  705       *
  706       *  Whether the allocator is moved depends on the allocator traits.
  707       */
  708      vector&
  709      operator=(vector&& __x) noexcept(_Alloc_traits::_S_nothrow_move())
  710      {
  711        constexpr bool __move_storage =
  712          _Alloc_traits::_S_propagate_on_move_assign()
  713          || _Alloc_traits::_S_always_equal();
  714        _M_move_assign(std::move(__x), __bool_constant<__move_storage>());
  715        return *this;
  716      }
  717 
  718      /**
  719       *  @brief  %Vector list assignment operator.
  720       *  @param  __l  An initializer_list.
  721       *
  722       *  This function fills a %vector with copies of the elements in the
  723       *  initializer list @a __l.
  724       *
  725       *  Note that the assignment completely changes the %vector and
  726       *  that the resulting %vector's size is the same as the number
  727       *  of elements assigned.
  728       */
  729      vector&
  730      operator=(initializer_list<value_type> __l)
  731      {
  732        this->_M_assign_aux(__l.begin(), __l.end(),
  733                            random_access_iterator_tag());
  734        return *this;
  735      }
  736#endif
  737 
  738      /**
  739       *  @brief  Assigns a given value to a %vector.
  740       *  @param  __n  Number of elements to be assigned.
  741       *  @param  __val  Value to be assigned.
  742       *
  743       *  This function fills a %vector with @a __n copies of the given
  744       *  value.  Note that the assignment completely changes the
  745       *  %vector and that the resulting %vector's size is the same as
  746       *  the number of elements assigned.
  747       */
  748      void
  749      assign(size_type __n, const value_type& __val)
  750      { _M_fill_assign(__n, __val); }
  751 
  752      /**
  753       *  @brief  Assigns a range to a %vector.
  754       *  @param  __first  An input iterator.
  755       *  @param  __last   An input iterator.
  756       *
  757       *  This function fills a %vector with copies of the elements in the
  758       *  range [__first,__last).
  759       *
  760       *  Note that the assignment completely changes the %vector and
  761       *  that the resulting %vector's size is the same as the number
  762       *  of elements assigned.
  763       */
  764#if __cplusplus >= 201103L
  765      template<typename _InputIterator,
  766               typename = std::_RequireInputIter<_InputIterator>>
  767        void
  768        assign(_InputIterator __first, _InputIterator __last)
  769        { _M_assign_dispatch(__first, __last, __false_type()); }
  770#else
  771      template<typename _InputIterator>
  772        void
  773        assign(_InputIterator __first, _InputIterator __last)
  774        {
  775          // Check whether it's an integral type.  If so, it's not an iterator.
  776          typedef typename std::__is_integer<_InputIterator>::__type _Integral;
  777          _M_assign_dispatch(__first, __last, _Integral());
  778        }
  779#endif
  780 
  781#if __cplusplus >= 201103L
  782      /**
  783       *  @brief  Assigns an initializer list to a %vector.
  784       *  @param  __l  An initializer_list.
  785       *
  786       *  This function fills a %vector with copies of the elements in the
  787       *  initializer list @a __l.
  788       *
  789       *  Note that the assignment completely changes the %vector and
  790       *  that the resulting %vector's size is the same as the number
  791       *  of elements assigned.
  792       */
  793      void
  794      assign(initializer_list<value_type> __l)
  795      {
  796        this->_M_assign_aux(__l.begin(), __l.end(),
  797                            random_access_iterator_tag());
  798      }
  799#endif
  800 
  801      /// Get a copy of the memory allocation object.
  802      using _Base::get_allocator;
  803 
  804      // iterators
  805      /**
  806       *  Returns a read/write iterator that points to the first
  807       *  element in the %vector.  Iteration is done in ordinary
  808       *  element order.
  809       */
  810      iterator
  811      begin() _GLIBCXX_NOEXCEPT
  812      { return iterator(this->_M_impl._M_start); }
  813 
  814      /**
  815       *  Returns a read-only (constant) iterator that points to the
  816       *  first element in the %vector.  Iteration is done in ordinary
  817       *  element order.
  818       */
  819      const_iterator
  820      begin() const _GLIBCXX_NOEXCEPT
  821      { return const_iterator(this->_M_impl._M_start); }
  822 
  823      /**
  824       *  Returns a read/write iterator that points one past the last
  825       *  element in the %vector.  Iteration is done in ordinary
  826       *  element order.
  827       */
  828      iterator
  829      end() _GLIBCXX_NOEXCEPT
  830      { return iterator(this->_M_impl._M_finish); }
  831 
  832      /**
  833       *  Returns a read-only (constant) iterator that points one past
  834       *  the last element in the %vector.  Iteration is done in
  835       *  ordinary element order.
  836       */
  837      const_iterator
  838      end() const _GLIBCXX_NOEXCEPT
  839      { return const_iterator(this->_M_impl._M_finish); }
  840 
  841      /**
  842       *  Returns a read/write reverse iterator that points to the
  843       *  last element in the %vector.  Iteration is done in reverse
  844       *  element order.
  845       */
  846      reverse_iterator
  847      rbegin() _GLIBCXX_NOEXCEPT
  848      { return reverse_iterator(end()); }
  849 
  850      /**
  851       *  Returns a read-only (constant) reverse iterator that points
  852       *  to the last element in the %vector.  Iteration is done in
  853       *  reverse element order.
  854       */
  855      const_reverse_iterator
  856      rbegin() const _GLIBCXX_NOEXCEPT
  857      { return const_reverse_iterator(end()); }
  858 
  859      /**
  860       *  Returns a read/write reverse iterator that points to one
  861       *  before the first element in the %vector.  Iteration is done
  862       *  in reverse element order.
  863       */
  864      reverse_iterator
  865      rend() _GLIBCXX_NOEXCEPT
  866      { return reverse_iterator(begin()); }
  867 
  868      /**
  869       *  Returns a read-only (constant) reverse iterator that points
  870       *  to one before the first element in the %vector.  Iteration
  871       *  is done in reverse element order.
  872       */
  873      const_reverse_iterator
  874      rend() const _GLIBCXX_NOEXCEPT
  875      { return const_reverse_iterator(begin()); }
  876 
  877#if __cplusplus >= 201103L
  878      /**
  879       *  Returns a read-only (constant) iterator that points to the
  880       *  first element in the %vector.  Iteration is done in ordinary
  881       *  element order.
  882       */
  883      const_iterator
  884      cbegin() const noexcept
  885      { return const_iterator(this->_M_impl._M_start); }
  886 
  887      /**
  888       *  Returns a read-only (constant) iterator that points one past
  889       *  the last element in the %vector.  Iteration is done in
  890       *  ordinary element order.
  891       */
  892      const_iterator
  893      cend() const noexcept
  894      { return const_iterator(this->_M_impl._M_finish); }
  895 
  896      /**
  897       *  Returns a read-only (constant) reverse iterator that points
  898       *  to the last element in the %vector.  Iteration is done in
  899       *  reverse element order.
  900       */
  901      const_reverse_iterator
  902      crbegin() const noexcept
  903      { return const_reverse_iterator(end()); }
  904 
  905      /**
  906       *  Returns a read-only (constant) reverse iterator that points
  907       *  to one before the first element in the %vector.  Iteration
  908       *  is done in reverse element order.
  909       */
  910      const_reverse_iterator
  911      crend() const noexcept
  912      { return const_reverse_iterator(begin()); }
  913#endif
  914 
  915      // [23.2.4.2] capacity
  916      /**  Returns the number of elements in the %vector.  */
  917      size_type
  918      size() const _GLIBCXX_NOEXCEPT
  919      { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
  920 
  921      /**  Returns the size() of the largest possible %vector.  */
  922      size_type
  923      max_size() const _GLIBCXX_NOEXCEPT
  924      { return _S_max_size(_M_get_Tp_allocator()); }
  925 
  926#if __cplusplus >= 201103L
  927      /**
  928       *  @brief  Resizes the %vector to the specified number of elements.
  929       *  @param  __new_size  Number of elements the %vector should contain.
  930       *
  931       *  This function will %resize the %vector to the specified
  932       *  number of elements.  If the number is smaller than the
  933       *  %vector's current size the %vector is truncated, otherwise
  934       *  default constructed elements are appended.
  935       */
  936      void
  937      resize(size_type __new_size)
  938      {
  939        if (__new_size > size())
  940          _M_default_append(__new_size - size());
  941        else if (__new_size < size())
  942          _M_erase_at_end(this->_M_impl._M_start + __new_size);
  943      }
  944 
  945      /**
  946       *  @brief  Resizes the %vector to the specified number of elements.
  947       *  @param  __new_size  Number of elements the %vector should contain.
  948       *  @param  __x  Data with which new elements should be populated.
  949       *
  950       *  This function will %resize the %vector to the specified
  951       *  number of elements.  If the number is smaller than the
  952       *  %vector's current size the %vector is truncated, otherwise
  953       *  the %vector is extended and new elements are populated with
  954       *  given data.
  955       */
  956      void
  957      resize(size_type __new_size, const value_type& __x)
  958      {
  959        if (__new_size > size())
  960          _M_fill_insert(end(), __new_size - size(), __x);
  961        else if (__new_size < size())
  962          _M_erase_at_end(this->_M_impl._M_start + __new_size);
  963      }
  964#else
  965      /**
  966       *  @brief  Resizes the %vector to the specified number of elements.
  967       *  @param  __new_size  Number of elements the %vector should contain.
  968       *  @param  __x  Data with which new elements should be populated.
  969       *
  970       *  This function will %resize the %vector to the specified
  971       *  number of elements.  If the number is smaller than the
  972       *  %vector's current size the %vector is truncated, otherwise
  973       *  the %vector is extended and new elements are populated with
  974       *  given data.
  975       */
  976      void
  977      resize(size_type __new_size, value_type __x = value_type())
  978      {
  979        if (__new_size > size())
  980          _M_fill_insert(end(), __new_size - size(), __x);
  981        else if (__new_size < size())
  982          _M_erase_at_end(this->_M_impl._M_start + __new_size);
  983      }
  984#endif
  985 
  986#if __cplusplus >= 201103L
  987      /**  A non-binding request to reduce capacity() to size().  */
  988      void
  989      shrink_to_fit()
  990      { _M_shrink_to_fit(); }
  991#endif
  992 
  993      /**
  994       *  Returns the total number of elements that the %vector can
  995       *  hold before needing to allocate more memory.
  996       */
  997      size_type
  998      capacity() const _GLIBCXX_NOEXCEPT
  999      { return size_type(this->_M_impl._M_end_of_storage
 1000                         - this->_M_impl._M_start); }
 1001 
 1002      /**
 1003       *  Returns true if the %vector is empty.  (Thus begin() would
 1004       *  equal end().)
 1005       */
 1006      _GLIBCXX_NODISCARD bool
 1007      empty() const _GLIBCXX_NOEXCEPT
 1008      { return begin() == end(); }
 1009 
 1010      /**
 1011       *  @brief  Attempt to preallocate enough memory for specified number of
 1012       *          elements.
 1013       *  @param  __n  Number of elements required.
 1014       *  @throw  std::length_error  If @a n exceeds @c max_size().
 1015       *
 1016       *  This function attempts to reserve enough memory for the
 1017       *  %vector to hold the specified number of elements.  If the
 1018       *  number requested is more than max_size(), length_error is
 1019       *  thrown.
 1020       *
 1021       *  The advantage of this function is that if optimal code is a
 1022       *  necessity and the user can determine the number of elements
 1023       *  that will be required, the user can reserve the memory in
 1024       *  %advance, and thus prevent a possible reallocation of memory
 1025       *  and copying of %vector data.
 1026       */
 1027      void
 1028      reserve(size_type __n);
 1029 
 1030      // element access
 1031      /**
 1032       *  @brief  Subscript access to the data contained in the %vector.
 1033       *  @param __n The index of the element for which data should be
 1034       *  accessed.
 1035       *  @return  Read/write reference to data.
 1036       *
 1037       *  This operator allows for easy, array-style, data access.
 1038       *  Note that data access with this operator is unchecked and
 1039       *  out_of_range lookups are not defined. (For checked lookups
 1040       *  see at().)
 1041       */
 1042      reference
 1043      operator[](size_type __n) _GLIBCXX_NOEXCEPT
 1044      {
 1045        __glibcxx_requires_subscript(__n);
 1046        return *(this->_M_impl._M_start + __n);
 1047      }
 1048 
 1049      /**
 1050       *  @brief  Subscript access to the data contained in the %vector.
 1051       *  @param __n The index of the element for which data should be
 1052       *  accessed.
 1053       *  @return  Read-only (constant) reference to data.
 1054       *
 1055       *  This operator allows for easy, array-style, data access.
 1056       *  Note that data access with this operator is unchecked and
 1057       *  out_of_range lookups are not defined. (For checked lookups
 1058       *  see at().)
 1059       */
 1060      const_reference
 1061      operator[](size_type __n) const _GLIBCXX_NOEXCEPT
 1062      {
 1063        __glibcxx_requires_subscript(__n);
 1064        return *(this->_M_impl._M_start + __n);
 1065      }
 1066 
 1067    protected:
 1068      /// Safety check used only from at().
 1069      void
 1070      _M_range_check(size_type __n) const
 1071      {
 1072        if (__n >= this->size())
 1073          __throw_out_of_range_fmt(__N("vector::_M_range_check: __n "
 1074                                       "(which is %zu) >= this->size() "
 1075                                       "(which is %zu)"),
 1076                                   __n, this->size());
 1077      }
 1078 
 1079    public:
 1080      /**
 1081       *  @brief  Provides access to the data contained in the %vector.
 1082       *  @param __n The index of the element for which data should be
 1083       *  accessed.
 1084       *  @return  Read/write reference to data.
 1085       *  @throw  std::out_of_range  If @a __n is an invalid index.
 1086       *
 1087       *  This function provides for safer data access.  The parameter
 1088       *  is first checked that it is in the range of the vector.  The
 1089       *  function throws out_of_range if the check fails.
 1090       */
 1091      reference
 1092      at(size_type __n)
 1093      {
 1094        _M_range_check(__n);
 1095        return (*this)[__n];
 1096      }
 1097 
 1098      /**
 1099       *  @brief  Provides access to the data contained in the %vector.
 1100       *  @param __n The index of the element for which data should be
 1101       *  accessed.
 1102       *  @return  Read-only (constant) reference to data.
 1103       *  @throw  std::out_of_range  If @a __n is an invalid index.
 1104       *
 1105       *  This function provides for safer data access.  The parameter
 1106       *  is first checked that it is in the range of the vector.  The
 1107       *  function throws out_of_range if the check fails.
 1108       */
 1109      const_reference
 1110      at(size_type __n) const
 1111      {
 1112        _M_range_check(__n);
 1113        return (*this)[__n];
 1114      }
 1115 
 1116      /**
 1117       *  Returns a read/write reference to the data at the first
 1118       *  element of the %vector.
 1119       */
 1120      reference
 1121      front() _GLIBCXX_NOEXCEPT
 1122      {
 1123        __glibcxx_requires_nonempty();
 1124        return *begin();
 1125      }
 1126 
 1127      /**
 1128       *  Returns a read-only (constant) reference to the data at the first
 1129       *  element of the %vector.
 1130       */
 1131      const_reference
 1132      front() const _GLIBCXX_NOEXCEPT
 1133      {
 1134        __glibcxx_requires_nonempty();
 1135        return *begin();
 1136      }
 1137 
 1138      /**
 1139       *  Returns a read/write reference to the data at the last
 1140       *  element of the %vector.
 1141       */
 1142      reference
 1143      back() _GLIBCXX_NOEXCEPT
 1144      {
 1145        __glibcxx_requires_nonempty();
 1146        return *(end() - 1);
 1147      }
 1148 
 1149      /**
 1150       *  Returns a read-only (constant) reference to the data at the
 1151       *  last element of the %vector.
 1152       */
 1153      const_reference
 1154      back() const _GLIBCXX_NOEXCEPT
 1155      {
 1156        __glibcxx_requires_nonempty();
 1157        return *(end() - 1);
 1158      }
 1159 
 1160      // _GLIBCXX_RESOLVE_LIB_DEFECTS
 1161      // DR 464. Suggestion for new member functions in standard containers.
 1162      // data access
 1163      /**
 1164       *   Returns a pointer such that [data(), data() + size()) is a valid
 1165       *   range.  For a non-empty %vector, data() == &front().
 1166       */
 1167      _Tp*
 1168      data() _GLIBCXX_NOEXCEPT
 1169      { return _M_data_ptr(this->_M_impl._M_start); }
 1170 
 1171      const _Tp*
 1172      data() const _GLIBCXX_NOEXCEPT
 1173      { return _M_data_ptr(this->_M_impl._M_start); }
 1174 
 1175      // [23.2.4.3] modifiers
 1176      /**
 1177       *  @brief  Add data to the end of the %vector.
 1178       *  @param  __x  Data to be added.
 1179       *
 1180       *  This is a typical stack operation.  The function creates an
 1181       *  element at the end of the %vector and assigns the given data
 1182       *  to it.  Due to the nature of a %vector this operation can be
 1183       *  done in constant time if the %vector has preallocated space
 1184       *  available.
 1185       */
 1186      void
 1187      push_back(const value_type& __x)
 1188      {
 1189        if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
 1190          {
 1191            _GLIBCXX_ASAN_ANNOTATE_GROW(1);
 1192            _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
 1193                                     __x);
 1194            ++this->_M_impl._M_finish;
 1195            _GLIBCXX_ASAN_ANNOTATE_GREW(1);
 1196          }
 1197        else
 1198          _M_realloc_insert(end(), __x);
 1199      }
 1200 
 1201#if __cplusplus >= 201103L
 1202      void
 1203      push_back(value_type&& __x)
 1204      { emplace_back(std::move(__x)); }
 1205 
 1206      template<typename... _Args>
 1207#if __cplusplus > 201402L
 1208        reference
 1209#else
 1210        void
 1211#endif
 1212        emplace_back(_Args&&... __args);
 1213#endif
 1214 
 1215      /**
 1216       *  @brief  Removes last element.
 1217       *
 1218       *  This is a typical stack operation. It shrinks the %vector by one.
 1219       *
 1220       *  Note that no data is returned, and if the last element's
 1221       *  data is needed, it should be retrieved before pop_back() is
 1222       *  called.
 1223       */
 1224      void
 1225      pop_back() _GLIBCXX_NOEXCEPT
 1226      {
 1227        __glibcxx_requires_nonempty();
 1228        --this->_M_impl._M_finish;
 1229        _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
 1230        _GLIBCXX_ASAN_ANNOTATE_SHRINK(1);
 1231      }
 1232 
 1233#if __cplusplus >= 201103L
 1234      /**
 1235       *  @brief  Inserts an object in %vector before specified iterator.
 1236       *  @param  __position  A const_iterator into the %vector.
 1237       *  @param  __args  Arguments.
 1238       *  @return  An iterator that points to the inserted data.
 1239       *
 1240       *  This function will insert an object of type T constructed
 1241       *  with T(std::forward<Args>(args)...) before the specified location.
 1242       *  Note that this kind of operation could be expensive for a %vector
 1243       *  and if it is frequently used the user should consider using
 1244       *  std::list.
 1245       */
 1246      template<typename... _Args>
 1247        iterator
 1248        emplace(const_iterator __position, _Args&&... __args)
 1249        { return _M_emplace_aux(__position, std::forward<_Args>(__args)...); }
 1250 
 1251      /**
 1252       *  @brief  Inserts given value into %vector before specified iterator.
 1253       *  @param  __position  A const_iterator into the %vector.
 1254       *  @param  __x  Data to be inserted.
 1255       *  @return  An iterator that points to the inserted data.
 1256       *
 1257       *  This function will insert a copy of the given value before
 1258       *  the specified location.  Note that this kind of operation
 1259       *  could be expensive for a %vector and if it is frequently
 1260       *  used the user should consider using std::list.
 1261       */
 1262      iterator
 1263      insert(const_iterator __position, const value_type& __x);
 1264#else
 1265      /**
 1266       *  @brief  Inserts given value into %vector before specified iterator.
 1267       *  @param  __position  An iterator into the %vector.
 1268       *  @param  __x  Data to be inserted.
 1269       *  @return  An iterator that points to the inserted data.
 1270       *
 1271       *  This function will insert a copy of the given value before
 1272       *  the specified location.  Note that this kind of operation
 1273       *  could be expensive for a %vector and if it is frequently
 1274       *  used the user should consider using std::list.
 1275       */
 1276      iterator
 1277      insert(iterator __position, const value_type& __x);
 1278#endif
 1279 
 1280#if __cplusplus >= 201103L
 1281      /**
 1282       *  @brief  Inserts given rvalue into %vector before specified iterator.
 1283       *  @param  __position  A const_iterator into the %vector.
 1284       *  @param  __x  Data to be inserted.
 1285       *  @return  An iterator that points to the inserted data.
 1286       *
 1287       *  This function will insert a copy of the given rvalue before
 1288       *  the specified location.  Note that this kind of operation
 1289       *  could be expensive for a %vector and if it is frequently
 1290       *  used the user should consider using std::list.
 1291       */
 1292      iterator
 1293      insert(const_iterator __position, value_type&& __x)
 1294      { return _M_insert_rval(__position, std::move(__x)); }
 1295 
 1296      /**
 1297       *  @brief  Inserts an initializer_list into the %vector.
 1298       *  @param  __position  An iterator into the %vector.
 1299       *  @param  __l  An initializer_list.
 1300       *
 1301       *  This function will insert copies of the data in the
 1302       *  initializer_list @a l into the %vector before the location
 1303       *  specified by @a position.
 1304       *
 1305       *  Note that this kind of operation could be expensive for a
 1306       *  %vector and if it is frequently used the user should
 1307       *  consider using std::list.
 1308       */
 1309      iterator
 1310      insert(const_iterator __position, initializer_list<value_type> __l)
 1311      {
 1312        auto __offset = __position - cbegin();
 1313        _M_range_insert(begin() + __offset, __l.begin(), __l.end(),
 1314                        std::random_access_iterator_tag());
 1315        return begin() + __offset;
 1316      }
 1317#endif
 1318 
 1319#if __cplusplus >= 201103L
 1320      /**
 1321       *  @brief  Inserts a number of copies of given data into the %vector.
 1322       *  @param  __position  A const_iterator into the %vector.
 1323       *  @param  __n  Number of elements to be inserted.
 1324       *  @param  __x  Data to be inserted.
 1325       *  @return  An iterator that points to the inserted data.
 1326       *
 1327       *  This function will insert a specified number of copies of
 1328       *  the given data before the location specified by @a position.
 1329       *
 1330       *  Note that this kind of operation could be expensive for a
 1331       *  %vector and if it is frequently used the user should
 1332       *  consider using std::list.
 1333       */
 1334      iterator
 1335      insert(const_iterator __position, size_type __n, const value_type& __x)
 1336      {
 1337        difference_type __offset = __position - cbegin();
 1338        _M_fill_insert(begin() + __offset, __n, __x);
 1339        return begin() + __offset;
 1340      }
 1341#else
 1342      /**
 1343       *  @brief  Inserts a number of copies of given data into the %vector.
 1344       *  @param  __position  An iterator into the %vector.
 1345       *  @param  __n  Number of elements to be inserted.
 1346       *  @param  __x  Data to be inserted.
 1347       *
 1348       *  This function will insert a specified number of copies of
 1349       *  the given data before the location specified by @a position.
 1350       *
 1351       *  Note that this kind of operation could be expensive for a
 1352       *  %vector and if it is frequently used the user should
 1353       *  consider using std::list.
 1354       */
 1355      void
 1356      insert(iterator __position, size_type __n, const value_type& __x)
 1357      { _M_fill_insert(__position, __n, __x); }
 1358#endif
 1359 
 1360#if __cplusplus >= 201103L
 1361      /**
 1362       *  @brief  Inserts a range into the %vector.
 1363       *  @param  __position  A const_iterator into the %vector.
 1364       *  @param  __first  An input iterator.
 1365       *  @param  __last   An input iterator.
 1366       *  @return  An iterator that points to the inserted data.
 1367       *
 1368       *  This function will insert copies of the data in the range
 1369       *  [__first,__last) into the %vector before the location specified
 1370       *  by @a pos.
 1371       *
 1372       *  Note that this kind of operation could be expensive for a
 1373       *  %vector and if it is frequently used the user should
 1374       *  consider using std::list.
 1375       */
 1376      template<typename _InputIterator,
 1377               typename = std::_RequireInputIter<_InputIterator>>
 1378        iterator
 1379        insert(const_iterator __position, _InputIterator __first,
 1380               _InputIterator __last)
 1381        {
 1382          difference_type __offset = __position - cbegin();
 1383          _M_insert_dispatch(begin() + __offset,
 1384                             __first, __last, __false_type());
 1385          return begin() + __offset;
 1386        }
 1387#else
 1388      /**
 1389       *  @brief  Inserts a range into the %vector.
 1390       *  @param  __position  An iterator into the %vector.
 1391       *  @param  __first  An input iterator.
 1392       *  @param  __last   An input iterator.
 1393       *
 1394       *  This function will insert copies of the data in the range
 1395       *  [__first,__last) into the %vector before the location specified
 1396       *  by @a pos.
 1397       *
 1398       *  Note that this kind of operation could be expensive for a
 1399       *  %vector and if it is frequently used the user should
 1400       *  consider using std::list.
 1401       */
 1402      template<typename _InputIterator>
 1403        void
 1404        insert(iterator __position, _InputIterator __first,
 1405               _InputIterator __last)
 1406        {
 1407          // Check whether it's an integral type.  If so, it's not an iterator.
 1408          typedef typename std::__is_integer<_InputIterator>::__type _Integral;
 1409          _M_insert_dispatch(__position, __first, __last, _Integral());
 1410        }
 1411#endif
 1412 
 1413      /**
 1414       *  @brief  Remove element at given position.
 1415       *  @param  __position  Iterator pointing to element to be erased.
 1416       *  @return  An iterator pointing to the next element (or end()).
 1417       *
 1418       *  This function will erase the element at the given position and thus
 1419       *  shorten the %vector by one.
 1420       *
 1421       *  Note This operation could be expensive and if it is
 1422       *  frequently used the user should consider using std::list.
 1423       *  The user is also cautioned that this function only erases
 1424       *  the element, and that if the element is itself a pointer,
 1425       *  the pointed-to memory is not touched in any way.  Managing
 1426       *  the pointer is the user's responsibility.
 1427       */
 1428      iterator
 1429#if __cplusplus >= 201103L
 1430      erase(const_iterator __position)
 1431      { return _M_erase(begin() + (__position - cbegin())); }
 1432#else
 1433      erase(iterator __position)
 1434      { return _M_erase(__position); }
 1435#endif
 1436 
 1437      /**
 1438       *  @brief  Remove a range of elements.
 1439       *  @param  __first  Iterator pointing to the first element to be erased.
 1440       *  @param  __last  Iterator pointing to one past the last element to be
 1441       *                  erased.
 1442       *  @return  An iterator pointing to the element pointed to by @a __last
 1443       *           prior to erasing (or end()).
 1444       *
 1445       *  This function will erase the elements in the range
 1446       *  [__first,__last) and shorten the %vector accordingly.
 1447       *
 1448       *  Note This operation could be expensive and if it is
 1449       *  frequently used the user should consider using std::list.
 1450       *  The user is also cautioned that this function only erases
 1451       *  the elements, and that if the elements themselves are
 1452       *  pointers, the pointed-to memory is not touched in any way.
 1453       *  Managing the pointer is the user's responsibility.
 1454       */
 1455      iterator
 1456#if __cplusplus >= 201103L
 1457      erase(const_iterator __first, const_iterator __last)
 1458      {
 1459        const auto __beg = begin();
 1460        const auto __cbeg = cbegin();
 1461        return _M_erase(__beg + (__first - __cbeg), __beg + (__last - __cbeg));
 1462      }
 1463#else
 1464      erase(iterator __first, iterator __last)
 1465      { return _M_erase(__first, __last); }
 1466#endif
 1467 
 1468      /**
 1469       *  @brief  Swaps data with another %vector.
 1470       *  @param  __x  A %vector of the same element and allocator types.
 1471       *
 1472       *  This exchanges the elements between two vectors in constant time.
 1473       *  (Three pointers, so it should be quite fast.)
 1474       *  Note that the global std::swap() function is specialized such that
 1475       *  std::swap(v1,v2) will feed to this function.
 1476       *
 1477       *  Whether the allocators are swapped depends on the allocator traits.
 1478       */
 1479      void
 1480      swap(vector& __x) _GLIBCXX_NOEXCEPT
 1481      {
 1482#if __cplusplus >= 201103L
 1483        __glibcxx_assert(_Alloc_traits::propagate_on_container_swap::value
 1484                         || _M_get_Tp_allocator() == __x._M_get_Tp_allocator());
 1485#endif
 1486        this->_M_impl._M_swap_data(__x._M_impl);
 1487        _Alloc_traits::_S_on_swap(_M_get_Tp_allocator(),
 1488                                  __x._M_get_Tp_allocator());
 1489      }
 1490 
 1491      /**
 1492       *  Erases all the elements.  Note that this function only erases the
 1493       *  elements, and that if the elements themselves are pointers, the
 1494       *  pointed-to memory is not touched in any way.  Managing the pointer is
 1495       *  the user's responsibility.
 1496       */
 1497      void
 1498      clear() _GLIBCXX_NOEXCEPT
 1499      { _M_erase_at_end(this->_M_impl._M_start); }
 1500 
 1501    protected:
 1502      /**
 1503       *  Memory expansion handler.  Uses the member allocation function to
 1504       *  obtain @a n bytes of memory, and then copies [first,last) into it.
 1505       */
 1506      template<typename _ForwardIterator>
 1507        pointer
 1508        _M_allocate_and_copy(size_type __n,
 1509                             _ForwardIterator __first, _ForwardIterator __last)
 1510        {
 1511          pointer __result = this->_M_allocate(__n);
 1512          __try
 1513            {
 1514              std::__uninitialized_copy_a(__first, __last, __result,
 1515                                          _M_get_Tp_allocator());
 1516              return __result;
 1517            }
 1518          __catch(...)
 1519            {
 1520              _M_deallocate(__result, __n);
 1521              __throw_exception_again;
 1522            }
 1523        }
 1524 
 1525 
 1526      // Internal constructor functions follow.
 1527 
 1528      // Called by the range constructor to implement [23.1.1]/9
 1529 
 1530#if __cplusplus < 201103L
 1531      // _GLIBCXX_RESOLVE_LIB_DEFECTS
 1532      // 438. Ambiguity in the "do the right thing" clause
 1533      template<typename _Integer>
 1534        void
 1535        _M_initialize_dispatch(_Integer __n, _Integer __value, __true_type)
 1536        {
 1537          this->_M_impl._M_start = _M_allocate(_S_check_init_len(
 1538                static_cast<size_type>(__n), _M_get_Tp_allocator()));
 1539          this->_M_impl._M_end_of_storage =
 1540            this->_M_impl._M_start + static_cast<size_type>(__n);
 1541          _M_fill_initialize(static_cast<size_type>(__n), __value);
 1542        }
 1543 
 1544      // Called by the range constructor to implement [23.1.1]/9
 1545      template<typename _InputIterator>
 1546        void
 1547        _M_initialize_dispatch(_InputIterator __first, _InputIterator __last,
 1548                               __false_type)
 1549        {
 1550          _M_range_initialize(__first, __last,
 1551                              std::__iterator_category(__first));
 1552        }
 1553#endif
 1554 
 1555      // Called by the second initialize_dispatch above
 1556      template<typename _InputIterator>
 1557        void
 1558        _M_range_initialize(_InputIterator __first, _InputIterator __last,
 1559                            std::input_iterator_tag)
 1560        {
 1561          __try {
 1562            for (; __first != __last; ++__first)
 1563#if __cplusplus >= 201103L
 1564              emplace_back(*__first);
 1565#else
 1566              push_back(*__first);
 1567#endif
 1568          } __catch(...) {
 1569            clear();
 1570            __throw_exception_again;
 1571          }
 1572        }
 1573 
 1574      // Called by the second initialize_dispatch above
 1575      template<typename _ForwardIterator>
 1576        void
 1577        _M_range_initialize(_ForwardIterator __first, _ForwardIterator __last,
 1578                            std::forward_iterator_tag)
 1579        {
 1580          const size_type __n = std::distance(__first, __last);
 1581          this->_M_impl._M_start
 1582            = this->_M_allocate(_S_check_init_len(__n, _M_get_Tp_allocator()));
 1583          this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
 1584          this->_M_impl._M_finish =
 1585            std::__uninitialized_copy_a(__first, __last,
 1586                                        this->_M_impl._M_start,
 1587                                        _M_get_Tp_allocator());
 1588        }
 1589 
 1590      // Called by the first initialize_dispatch above and by the
 1591      // vector(n,value,a) constructor.
 1592      void
 1593      _M_fill_initialize(size_type __n, const value_type& __value)
 1594      {
 1595        this->_M_impl._M_finish =
 1596          std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value,
 1597                                        _M_get_Tp_allocator());
 1598      }
 1599 
 1600#if __cplusplus >= 201103L
 1601      // Called by the vector(n) constructor.
 1602      void
 1603      _M_default_initialize(size_type __n)
 1604      {
 1605        this->_M_impl._M_finish =
 1606          std::__uninitialized_default_n_a(this->_M_impl._M_start, __n,
 1607                                           _M_get_Tp_allocator());
 1608      }
 1609#endif
 1610 
 1611      // Internal assign functions follow.  The *_aux functions do the actual
 1612      // assignment work for the range versions.
 1613 
 1614      // Called by the range assign to implement [23.1.1]/9
 1615 
 1616      // _GLIBCXX_RESOLVE_LIB_DEFECTS
 1617      // 438. Ambiguity in the "do the right thing" clause
 1618      template<typename _Integer>
 1619        void
 1620        _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
 1621        { _M_fill_assign(__n, __val); }
 1622 
 1623      // Called by the range assign to implement [23.1.1]/9
 1624      template<typename _InputIterator>
 1625        void
 1626        _M_assign_dispatch(_InputIterator __first, _InputIterator __last,
 1627                           __false_type)
 1628        { _M_assign_aux(__first, __last, std::__iterator_category(__first)); }
 1629 
 1630      // Called by the second assign_dispatch above
 1631      template<typename _InputIterator>
 1632        void
 1633        _M_assign_aux(_InputIterator __first, _InputIterator __last,
 1634                      std::input_iterator_tag);
 1635 
 1636      // Called by the second assign_dispatch above
 1637      template<typename _ForwardIterator>
 1638        void
 1639        _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
 1640                      std::forward_iterator_tag);
 1641 
 1642      // Called by assign(n,t), and the range assign when it turns out
 1643      // to be the same thing.
 1644      void
 1645      _M_fill_assign(size_type __n, const value_type& __val);
 1646 
 1647      // Internal insert functions follow.
 1648 
 1649      // Called by the range insert to implement [23.1.1]/9
 1650 
 1651      // _GLIBCXX_RESOLVE_LIB_DEFECTS
 1652      // 438. Ambiguity in the "do the right thing" clause
 1653      template<typename _Integer>
 1654        void
 1655        _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val,
 1656                           __true_type)
 1657        { _M_fill_insert(__pos, __n, __val); }
 1658 
 1659      // Called by the range insert to implement [23.1.1]/9
 1660      template<typename _InputIterator>
 1661        void
 1662        _M_insert_dispatch(iterator __pos, _InputIterator __first,
 1663                           _InputIterator __last, __false_type)
 1664        {
 1665          _M_range_insert(__pos, __first, __last,
 1666                          std::__iterator_category(__first));
 1667        }
 1668 
 1669      // Called by the second insert_dispatch above
 1670      template<typename _InputIterator>
 1671        void
 1672        _M_range_insert(iterator __pos, _InputIterator __first,
 1673                        _InputIterator __last, std::input_iterator_tag);
 1674 
 1675      // Called by the second insert_dispatch above
 1676      template<typename _ForwardIterator>
 1677        void
 1678        _M_range_insert(iterator __pos, _ForwardIterator __first,
 1679                        _ForwardIterator __last, std::forward_iterator_tag);
 1680 
 1681      // Called by insert(p,n,x), and the range insert when it turns out to be
 1682      // the same thing.
 1683      void
 1684      _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);
 1685 
 1686#if __cplusplus >= 201103L
 1687      // Called by resize(n).
 1688      void
 1689      _M_default_append(size_type __n);
 1690 
 1691      bool
 1692      _M_shrink_to_fit();
 1693#endif
 1694 
 1695#if __cplusplus < 201103L
 1696      // Called by insert(p,x)
 1697      void
 1698      _M_insert_aux(iterator __position, const value_type& __x);
 1699 
 1700      void
 1701      _M_realloc_insert(iterator __position, const value_type& __x);
 1702#else
 1703      // A value_type object constructed with _Alloc_traits::construct()
 1704      // and destroyed with _Alloc_traits::destroy().
 1705      struct _Temporary_value
 1706      {
 1707        template<typename... _Args>
 1708          explicit
 1709          _Temporary_value(vector* __vec, _Args&&... __args) : _M_this(__vec)
 1710          {
 1711            _Alloc_traits::construct(_M_this->_M_impl, _M_ptr(),
 1712                                     std::forward<_Args>(__args)...);
 1713          }
 1714 
 1715        ~~_Temporary_value()
 1716        { _Alloc_traits::destroy(_M_this->_M_impl, _M_ptr()); }
 1717 
 1718        value_type&
 1719        _M_val() { return *_M_ptr(); }
 1720 
 1721      private:
 1722        _Tp*
 1723        _M_ptr() { return reinterpret_cast<_Tp*>(&__buf); }
 1724 
 1725        vector* _M_this;
 1726        typename aligned_storage<sizeof(_Tp), alignof(_Tp)>::type __buf;
 1727      };
 1728 
 1729      // Called by insert(p,x) and other functions when insertion needs to
 1730      // reallocate or move existing elements. _Arg is either _Tp& or _Tp.
 1731      template<typename _Arg>
 1732        void
 1733        _M_insert_aux(iterator __position, _Arg&& __arg);
 1734 
 1735      template<typename... _Args>
 1736        void
 1737        _M_realloc_insert(iterator __position, _Args&&... __args);
 1738 
 1739      // Either move-construct at the end, or forward to _M_insert_aux.
 1740      iterator
 1741      _M_insert_rval(const_iterator __position, value_type&& __v);
 1742 
 1743      // Try to emplace at the end, otherwise forward to _M_insert_aux.
 1744      template<typename... _Args>
 1745        iterator
 1746        _M_emplace_aux(const_iterator __position, _Args&&... __args);
 1747 
 1748      // Emplacing an rvalue of the correct type can use _M_insert_rval.
 1749      iterator
 1750      _M_emplace_aux(const_iterator __position, value_type&& __v)
 1751      { return _M_insert_rval(__position, std::move(__v)); }
 1752#endif
 1753 
 1754      // Called by _M_fill_insert, _M_insert_aux etc.
 1755      size_type
 1756      _M_check_len(size_type __n, const char* __s) const
 1757      {
 1758        if (max_size() - size() < __n)
 1759          __throw_length_error(__N(__s));
 1760 
 1761        const size_type __len = size() + (std::max)(size(), __n);
 1762        return (__len < size() || __len > max_size()) ? max_size() : __len;
 1763      }
 1764 
 1765      // Called by constructors to check initial size.
 1766      static size_type
 1767      _S_check_init_len(size_type __n, const allocator_type& __a)
 1768      {
 1769        if (__n > _S_max_size(_Tp_alloc_type(__a)))
 1770          __throw_length_error(
 1771              __N("cannot create std::vector larger than max_size()"));
 1772        return __n;
 1773      }
 1774 
 1775      static size_type
 1776      _S_max_size(const _Tp_alloc_type& __a) _GLIBCXX_NOEXCEPT
 1777      {
 1778        // std::distance(begin(), end()) cannot be greater than PTRDIFF_MAX,
 1779        // and realistically we can't store more than PTRDIFF_MAX/sizeof(T)
 1780        // (even if std::allocator_traits::max_size says we can).
 1781        const size_t __diffmax
 1782          = __gnu_cxx::__numeric_traits<ptrdiff_t>::__max / sizeof(_Tp);
 1783        const size_t __allocmax = _Alloc_traits::max_size(__a);
 1784        return (std::min)(__diffmax, __allocmax);
 1785      }
 1786 
 1787      // Internal erase functions follow.
 1788 
 1789      // Called by erase(q1,q2), clear(), resize(), _M_fill_assign,
 1790      // _M_assign_aux.
 1791      void
 1792      _M_erase_at_end(pointer __pos) _GLIBCXX_NOEXCEPT
 1793      {
 1794        if (size_type __n = this->_M_impl._M_finish - __pos)
 1795          {
 1796            std::_Destroy(__pos, this->_M_impl._M_finish,
 1797                          _M_get_Tp_allocator());
 1798            this->_M_impl._M_finish = __pos;
 1799            _GLIBCXX_ASAN_ANNOTATE_SHRINK(__n);
 1800          }
 1801      }
 1802 
 1803      iterator
 1804      _M_erase(iterator __position);
 1805 
 1806      iterator
 1807      _M_erase(iterator __first, iterator __last);
 1808 
 1809#if __cplusplus >= 201103L
 1810    private:
 1811      // Constant-time move assignment when source object's memory can be
 1812      // moved, either because the source's allocator will move too
 1813      // or because the allocators are equal.
 1814      void
 1815      _M_move_assign(vector&& __x, true_type) noexcept
 1816      {
 1817        vector __tmp(get_allocator());
 1818        this->_M_impl._M_swap_data(__x._M_impl);
 1819        __tmp._M_impl._M_swap_data(__x._M_impl);
 1820        std::__alloc_on_move(_M_get_Tp_allocator(), __x._M_get_Tp_allocator());
 1821      }
 1822 
 1823      // Do move assignment when it might not be possible to move source
 1824      // object's memory, resulting in a linear-time operation.
 1825      void
 1826      _M_move_assign(vector&& __x, false_type)
 1827      {
 1828        if (__x._M_get_Tp_allocator() == this->_M_get_Tp_allocator())
 1829          _M_move_assign(std::move(__x), true_type());
 1830        else
 1831          {
 1832            // The rvalue's allocator cannot be moved and is not equal,
 1833            // so we need to individually move each element.
 1834            this->_M_assign_aux(std::make_move_iterator(__x.begin()),
 1835                                std::make_move_iterator(__x.end()),
 1836                                std::random_access_iterator_tag());
 1837            __x.clear();
 1838          }
 1839      }
 1840#endif
 1841 
 1842      template<typename _Up>
 1843        _Up*
 1844        _M_data_ptr(_Up* __ptr) const _GLIBCXX_NOEXCEPT
 1845        { return __ptr; }
 1846 
 1847#if __cplusplus >= 201103L
 1848      template<typename _Ptr>
 1849        typename std::pointer_traits<_Ptr>::element_type*
 1850        _M_data_ptr(_Ptr __ptr) const
 1851        { return empty() ? nullptr : std::__to_address(__ptr); }
 1852#else
 1853      template<typename _Up>
 1854        _Up*
 1855        _M_data_ptr(_Up* __ptr) _GLIBCXX_NOEXCEPT
 1856        { return __ptr; }
 1857 
 1858      template<typename _Ptr>
 1859        value_type*
 1860        _M_data_ptr(_Ptr __ptr)
 1861        { return empty() ? (value_type*)0 : __ptr.operator->(); }
 1862 
 1863      template<typename _Ptr>
 1864        const value_type*
 1865        _M_data_ptr(_Ptr __ptr) const
 1866        { return empty() ? (const value_type*)0 : __ptr.operator->(); }
 1867#endif
 1868    };
 1869 
 1870#if __cpp_deduction_guides >= 201606
 1871  template<typename _InputIterator, typename _ValT
 1872             = typename iterator_traits<_InputIterator>::value_type,
 1873           typename _Allocator = allocator<_ValT>,
 1874           typename = _RequireInputIter<_InputIterator>,
 1875           typename = _RequireAllocator<_Allocator>>
 1876    vector(_InputIterator, _InputIterator, _Allocator = _Allocator())
 1877      -> vector<_ValT, _Allocator>;
 1878#endif
 1879 
 1880  /**
 1881   *  @brief  Vector equality comparison.
 1882   *  @param  __x  A %vector.
 1883   *  @param  __y  A %vector of the same type as @a __x.
 1884   *  @return  True iff the size and elements of the vectors are equal.
 1885   *
 1886   *  This is an equivalence relation.  It is linear in the size of the
 1887   *  vectors.  Vectors are considered equivalent if their sizes are equal,
 1888   *  and if corresponding elements compare equal.
 1889  */
 1890  template<typename _Tp, typename _Alloc>
 1891    inline bool
 1892    operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
 1893    { return (__x.size() == __y.size()
 1894              && std::equal(__x.begin(), __x.end(), __y.begin())); }
 1895 
 1896#if __cpp_lib_three_way_comparison
 1897  /**
 1898   *  @brief  Vector ordering relation.
 1899   *  @param  __x  A `vector`.
 1900   *  @param  __y  A `vector` of the same type as `__x`.
 1901   *  @return  A value indicating whether `__x` is less than, equal to,
 1902   *           greater than, or incomparable with `__y`.
 1903   *
 1904   *  See `std::lexicographical_compare_three_way()` for how the determination
 1905   *  is made. This operator is used to synthesize relational operators like
 1906   *  `<` and `>=` etc.
 1907  */
 1908  template<typename _Tp, typename _Alloc>
 1909    inline __detail::__synth3way_t<_Tp>
 1910    operator<=>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
 1911    {
 1912      return std::lexicographical_compare_three_way(__x.begin(), __x.end(),
 1913                                                    __y.begin(), __y.end(),
 1914                                                    __detail::__synth3way);
 1915    }
 1916#else
 1917  /**
 1918   *  @brief  Vector ordering relation.
 1919   *  @param  __x  A %vector.
 1920   *  @param  __y  A %vector of the same type as @a __x.
 1921   *  @return  True iff @a __x is lexicographically less than @a __y.
 1922   *
 1923   *  This is a total ordering relation.  It is linear in the size of the
 1924   *  vectors.  The elements must be comparable with @c <.
 1925   *
 1926   *  See std::lexicographical_compare() for how the determination is made.
 1927  */
 1928  template<typename _Tp, typename _Alloc>
 1929    inline bool
 1930    operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
 1931    { return std::lexicographical_compare(__x.begin(), __x.end(),
 1932                                          __y.begin(), __y.end()); }
 1933 
 1934  /// Based on operator==
 1935  template<typename _Tp, typename _Alloc>
 1936    inline bool
 1937    operator!=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
 1938    { return !(__x == __y); }
 1939 
 1940  /// Based on operator<
 1941  template<typename _Tp, typename _Alloc>
 1942    inline bool
 1943    operator>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
 1944    { return __y < __x; }
 1945 
 1946  /// Based on operator<
 1947  template<typename _Tp, typename _Alloc>
 1948    inline bool
 1949    operator<=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
 1950    { return !(__y < __x); }
 1951 
 1952  /// Based on operator<
 1953  template<typename _Tp, typename _Alloc>
 1954    inline bool
 1955    operator>=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
 1956    { return !(__x < __y); }
 1957#endif // three-way comparison
 1958 
 1959  /// See std::vector::swap().
 1960  template<typename _Tp, typename _Alloc>
 1961    inline void
 1962    swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y)
 1963    _GLIBCXX_NOEXCEPT_IF(noexcept(__x.swap(__y)))
 1964    { __x.swap(__y); }
 1965 
 1966_GLIBCXX_END_NAMESPACE_CONTAINER
 1967 
 1968#if __cplusplus >= 201703L
 1969  namespace __detail::__variant
 1970  {
 1971    template<typename> struct _Never_valueless_alt; // see <variant>
 1972 
 1973    // Provide the strong exception-safety guarantee when emplacing a
 1974    // vector into a variant, but only if move assignment cannot throw.
 1975    template<typename _Tp, typename _Alloc>
 1976      struct _Never_valueless_alt<_GLIBCXX_STD_C::vector<_Tp, _Alloc>>
 1977      : std::is_nothrow_move_assignable<_GLIBCXX_STD_C::vector<_Tp, _Alloc>>
 1978      { };
 1979  }  // namespace __detail::__variant
 1980#endif // C++17
 1981 
 1982_GLIBCXX_END_NAMESPACE_VERSION
 1983} // namespace std
 1984 
 1985#endif /* _STL_VECTOR_H */