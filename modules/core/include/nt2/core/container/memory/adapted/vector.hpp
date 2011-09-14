//==============================================================================
//         Copyright 2003 - 2011   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_CORE_CONTAINER_MEMORY_ADAPTED_VECTOR_HPP
#define NT2_CORE_CONTAINER_MEMORY_ADAPTED_VECTOR_HPP

#include <vector>
#include <boost/mpl/size_t.hpp>
#include <nt2/core/container/meta/dimensions_of.hpp>
#include <nt2/core/container/meta/reference.hpp>

//==============================================================================
// Fill out the Buffer concepts for std::vector
//==============================================================================
namespace nt2 { namespace meta
{
  template<typename T, typename A>
  struct  dimensions_of< std::vector<T,A> > 
        : boost::mpl::size_t<1 + dimensions_of<T>::value> 
  {};

  //============================================================================
  // reference_ specialization
  //============================================================================
  template<typename T, typename A, std::size_t Level> 
  struct reference_<std::vector<T,A>,Level>
  {
    typedef typename std::vector<T,A>::value_type  base;
    typedef typename reference_<base,Level-1>::type type;
  };

  template<typename T, typename A> 
  struct reference_<std::vector<T,A>,1UL>
  {
    typedef typename std::vector<T,A>::reference type;
  };

  //============================================================================
  // const_reference_ specialization
  //============================================================================
  template<typename T, typename A, std::size_t Level> 
  struct const_reference_<std::vector<T,A>,Level>
  {
    typedef typename std::vector<T,A>::value_type        base;
    typedef typename const_reference_<base,Level-1>::type type;
  };

  template<typename T, typename A> 
  struct const_reference_<std::vector<T,A>,1UL>
  {
    typedef typename std::vector<T,A>::const_reference type;
  };
} }

namespace nt2 { namespace details
{
  template<std::size_t Dims> struct build
  {
    template<typename T, typename A, typename Sizes>
    static inline void apply( std::vector<T,A>& v, Sizes const& s)
    {
      v.resize(boost::fusion::at_c<Dims-1>(s)); 
      inner_apply(v,s,boost::mpl::size_t<Dims>());
    }

    template<typename T, typename A, typename Sizes, typename N> static inline 
    void inner_apply( std::vector<T,A>& v, Sizes const& s, N)
    {
      for(std::size_t i=0;i<boost::fusion::at_c<Dims-1>(s);++i) 
        build<Dims-1>::apply(v[i],s);
    }
    
    template<typename T, typename A, typename Sizes> static inline 
    void inner_apply( std::vector<T,A>&, Sizes const&, boost::mpl::size_t<1> const&)
    {}
  };
} }

namespace nt2 { namespace memory
{
  //============================================================================
  // std::vector initialize - Part of Buffer Concept
  //============================================================================
  template<typename T, typename A, typename Sizes, typename Bases>
  inline void initialize( std::vector<T,A>& v, Sizes const& s, Bases const&)
  {
    //==========================================================================
    // Recursively allocate all level of the current std::vector
    //==========================================================================
    details::build<meta::dimensions_of<std::vector<T,A> >::value>::apply(v,s);
  }
} }
#endif
