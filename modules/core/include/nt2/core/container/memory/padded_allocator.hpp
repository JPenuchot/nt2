//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_CONTAINER_MEMORY_PADDED_ALLOCATOR_HPP_INCLUDED
#define NT2_CORE_CONTAINER_MEMORY_PADDED_ALLOCATOR_HPP_INCLUDED

#include <cstddef>
#include <boost/simd/sdk/memory/align_on.hpp>

namespace nt2 {  namespace memory
{
  //============================================================================
  /**!
   * padded_allocator is a stateful allocator adaptor that wraps an existing
   * allocator to force the alignment of its allocation
   **/
  //============================================================================
  template<typename Value, typename Allocator>
  struct padded_allocator : public Allocator
  {
    //==========================================================================
    // Allocator static expression conformance
    //==========================================================================
    typedef typename Allocator::value_type      value_type;
    typedef typename Allocator::pointer         pointer;
    typedef typename Allocator::const_pointer   const_pointer;
    typedef typename Allocator::reference       reference;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::size_type       size_type;
    typedef typename Allocator::difference_type difference_type;

    template<class Z> struct rebind
    {
      typedef typename Allocator::template rebind<Z>::other base;
      typedef padded_allocator<Value,base>                  other;
    };

    //==========================================================================
    // Ctor/dtor
    //==========================================================================
    padded_allocator( Value const& v, Allocator const& a )
                    : Allocator(a), value_(v)
    {}
    
    //==========================================================================
    // Transtyping constructor
    //==========================================================================
    template<class Z>
    padded_allocator( padded_allocator<Value, Z> const& src )
                    : Allocator(src), value_(src.value_)
    {}

    //==========================================================================
    // Alignment data
    //==========================================================================
    Value value_;

    ////////////////////////////////////////////////////////////////////////////
    // Memory handling
    ////////////////////////////////////////////////////////////////////////////
    pointer allocate( size_type s, const void* h = 0 )
    {
      return Allocator::allocate( boost::simd::memory::align_on(s,value_), h );
    }
  };

  //============================================================================
  /**!
   * Checks if two padded_allocator are equal. Such allocators are equal if and
   * only if they share the same pointee.
   **/
  //============================================================================    
  template<typename V, typename A>
  bool operator==(padded_allocator<V,A> const& l, padded_allocator<V,A> const& r)
  {
    return    (static_cast<A const&>(l) == static_cast<A const&>(r))
          &&  (l.value_ == r.value_);
  }

  //============================================================================
  /**!
   * Checks if two padded_allocator are non-equal. Such allocators are not equal
   * only if they share different pointees.
   **/
  //============================================================================    
  template<typename V, typename A>
  bool operator!=(padded_allocator<V,A> const& l, padded_allocator<V,A> const& r)
  {
    return !(l == r);
  }
} }

#endif
